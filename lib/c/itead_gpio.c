/**
 * @file itead_gpio.c
 *
 * Provide the implementation of Arduino API on gpio
 * 
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/3/11
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#include <linux/ioctl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <itead_gpio.h>
#include <itead_delay.h>
#include <itead_gpio_pin_map.h>
#include <itead_global.h>

//#define DEBUG
#include <itead_debug.h>


typedef struct PinPwmInfo{
    uint16_t 	pin_no;
	uint8_t 	duty;
	pthread_t	tid;
	uint8_t		state;
	uint32_t	high_us;
	uint32_t	low_us;
} PinPwmInfo;

#define CUREG   (*(cureg))

/*
 * PWM_FREQUENCY 	- pwm frequency.
 * PWM_PERIOD_US 	- pwm period in microsecond.
 * PIN_PWM_BASE_CNT - duty base number.
 * PIN_PWM_MAX 		- maxmial threads for pwm output. 
 */
#define PWM_FREQUENCY		(1000*1)		// 1kHz
#define PWM_PERIOD_US		((1.0/PWM_FREQUENCY)*(1000000.0))
#define PIN_PWM_BASE_CNT	255
#define PIN_PWM_USED		1
#define PIN_PWM_FREE		0
#define PIN_PWM_MAX			8


static PinPwmInfo           pin_pwm_infos[PIN_PWM_MAX];
static int                  fd_dev_mem = -1;
static volatile uint32_t *gpio_base;


/*
 * On BeagleBoneBlack , We use sysfs interface to control GPIO
 * With kernel config :
 *    > Device Driver
 *      > GPIO SUPPORT
 *        > /sys/class/gpio/... (sysfs interface) 
 */
#ifdef BOARD_BEAGLEBONEBLACK

/*
 * EXPORT_GPIO   	- bash command to export gpio with it's num 
 * UNEXPORT_GPIO 	- bash command to unexport gpio with it's num
 * DIRECTION_GPIO   - bash command to set gpio direction status
 * VALUE_GPIO 		- path to gpio value file
 */
#define EXPORT_GPIO "echo %d > /sys/class/gpio/export"
#define UNEXPORT_GPIO "echo %d > /sys/class/gpio/unexport"
#define DIRECTION_GPIO "echo %s > /sys/class/gpio/gpio%d/direction"
#define VALUE_GPIO "/sys/class/gpio/gpio%d/value"

/*
 * gpio_string[60]    - store the bash command to excute.
 * gpio_exported[132] - store 132 GPIOs' export status.
 *                          1-export , 0-unexport
 */
static unsigned char gpio_string[60];
static unsigned char gpio_exported[132] = {0};

/*
 * @name	: do_shell
 * @desc	: excute a bash command stored in *string
 * @param	: 
 * @return	: 1 -  if success.
 *			  0 -  if failed.
 */
static inline int do_shell(unsigned char * string)
{
    pid_t status;
    status = system(string);
    if (status == -1) {
        sdkerr("\nsystem call sh error.\n");
        return 0;
    }

    if (WIFEXITED(status) == 0) {
        sdkerr("\nbash command excute error.\n");
        return 0;
    }

    if (WEXITSTATUS(status) != 0) {
        sdkerr("\nbash command return status error.\n");
        sdkerr("return status is [0x%x].\n",status);
        return 0;
    }
    
    return 1;
}

/*
 * @name	: export_gpio
 * @desc	: export a gpio to sysfs interface
 *            if succeed ,the gpio is ready 
 * @param	: 
 * @return	: 1 -  if success.
 *			  0 -  if failed.
 */
static inline int export_gpio(unsigned int gpio_num)
{
    int status;
    /* export already */
    if (gpio_exported[gpio_num] == 1) {
        return 1;
    }

    sprintf(gpio_string,EXPORT_GPIO,gpio_num);
    status = do_shell(gpio_string);

    if (status == 1) {
        gpio_exported[gpio_num] = 1;
    }

    return status;
}

/*
 * @name	: unexport_gpio
 * @desc	: unexport a gpio from sysfs interface
 *            if succeed ,gpio can not be set further
 * @param	: 
 * @return	: 1 -  if success.
 *			  0 -  if failed.
 */
static inline int unexport_gpio(unsigned int gpio_num)
{
    int status;
    /* unexport already */
    if (gpio_exported[gpio_num] == 0) {
        return 1;
    }

    sprintf(gpio_string,UNEXPORT_GPIO,gpio_num);
    status = do_shell(gpio_string);

    if (status == 1) {
        gpio_exported[gpio_num] = 0;
    }

    return status;
}

/*
 * @name	: check_export_gpio
 * @desc	: check gpio export status 
 * @param	: 
 * @return	: 1 -  export 
 *			  0 -  unexport
 *            2 -  unknown status
 */
static inline int check_export_gpio(unsigned int gpio_num)
{
    FILE *f_gpio;
    sprintf(gpio_string,VALUE_GPIO,gpio_num);
    f_gpio = fopen(gpio_string,"r");
    if(f_gpio == NULL) {
        if(errno == ENOENT) {
            gpio_exported[gpio_num] = 0;
            return 0;
        } else {
            return 2;
        }
    }
    fclose(f_gpio);
    gpio_exported[gpio_num] = 1;
    return 1;
}

#endif

/*
 * @name	: gpio_mmap
 * @desc	: mmap to gpio base address
 * @param	: 
 * @return	: 1 -  if success.
 *			  0 - if failed.
 */
static inline int32_t gpio_mmap(void)
{
	/* open already */
	if (fd_dev_mem >= 0) {
		return 1;
	}
    if ((fd_dev_mem = open ("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) == -1) {
        printf("\nopen /dev/mem error !\n");
        return 0;
    }

#if defined(BOARD_ITEADUINO_PLUS) || defined(BOARD_ITEAD_CORE_EVB)
     if ((gpio_base = (volatile uint32_t *)mmap(0, 
            GPIO_SIZE, 
            PROT_READ|PROT_WRITE, 
            MAP_SHARED,
            fd_dev_mem, 
            GPIO_BASE-0x800)) == MAP_FAILED){
        printf("\nmmap error !\n");
        return 0;
    }
    gpio_base += 0x800/4;
#elif defined(BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    if ((gpio_base = (volatile uint32_t *)mmap(0, 
            GPIO_SIZE, 
            PROT_READ|PROT_WRITE, 
            MAP_SHARED,
            fd_dev_mem, 
            GPIO_BASE)) == MAP_FAILED){
        printf("\nmmap error !\n");
        return 0;
    }
#endif
	return 1;
}

/*
 * @name	: vertify_pin
 * @desc	: vertify pin for availability.
 * @param	: pin - pin number.
 * @return	: 1 if available, 0 if unavailable.
 */
static inline uint32_t vertify_pin(uint16_t pin)
{
	if(pin < PIN_MIN || pin > PIN_MAX) {
		return 0;
	}
	if (pnp[pin].port_no != PORT_NONE 
		&& pnp[pin].index != INDEX_NONE) {
		return 1;
	} else {
		return 0;
	}
}

/*
 * @name	: vertify_mode
 * @desc	: vertify mode for availability.
 * @param	: mode - INPUT or OUTPUT.
 * @return	: 1 if available, 0 if unavailable.
 */
static inline uint32_t vertify_mode(uint8_t mode)
{
	if(mode == INPUT || mode == OUTPUT) {
		return 1;
	}
	return 0;
}

/*
 * @name	: vertify_val
 * @desc	: vertify val for availability.
 * @param	: val - the value of pin.
 * @return	: 1 if available, 0 if unavailable.
 */
static inline uint32_t vertify_val(uint8_t val)
{
	if(val == HIGH || val == LOW) {
		return 1;
	}
	return 0;
}

/**
 * Set pin as INPUT or OUTPUT mode
 *
 * @param pin - the pin number
 * @param mode - only INPUT or OUTPUT can be accepted
 * @retval 0 - success
 * @retval 1 - fail
 */
uint32_t pinMode(uint16_t pin, uint8_t mode)
{
	uint32_t msg=0;
	volatile uint32_t *cureg;
    uint16_t port_no ;
	uint16_t index ;

	debug("\npinMode begin\n");
	
	if (!vertify_mode(mode)) {
		sdkerr("\nIllegal parameter : mode=%u\n",mode);
		return 1;
	}

	if (!vertify_pin(pin)) {
		sdkerr("\nIllegal parameter : pin=%u\n",pin);
		return 1;
	}
	
	if(!gpio_mmap()) {
		sdkerr("\ngpio_mmap failed!\n");
		return 1;
	}
	
	/* write register bo set mode */
#if defined(BOARD_ITEADUINO_PLUS) || defined(BOARD_ITEAD_CORE_EVB)
    /* find the register address need to operate */
    port_no = pnp[pin].port_no;
    index   = pnp[pin].index;
	cureg = (volatile uint32_t *)((uint32_t)gpio_base 
		+ port_no*0x24 + 4*(index/8));
    
	CUREG &= ~(0xF<<((index%8)*4));
	if( mode == INPUT ) {
		CUREG |= 0x0 << ((index%8)*4);
	} else if(mode == OUTPUT) {
		CUREG |= 0x1 << ((index%8)*4);
    }

#elif defined(BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    /* config pin as OUTPUT or INPUT */
   pin   = pnp[pin].index;
   cureg = gpio_base + GPIO_GPFSEL_OFFSET/4 + pin/10;
   CUREG &= ~(0x7 << (3*(pin%10)));
   if(mode == INPUT) {
       CUREG |=  (0x0 << (3*(pin%10)));
   } else {
       CUREG |=  (0x1 << (3*(pin%10)));
   }

#elif defined(BOARD_BEAGLEBONEBLACK)

   check_export_gpio(pin);
   
   if (!export_gpio(pin)) {
       sdkerr("\nexport_gpio failed!\n");
       return 1;
   }

   if (mode == INPUT) {
       sprintf(gpio_string,DIRECTION_GPIO,"in",pin);
   } else if(mode == OUTPUT) {
       sprintf(gpio_string,DIRECTION_GPIO,"out",pin);
   }
  
   if (!do_shell(gpio_string)) {
       sdkerr("\nset gpio direction failed!\n");
   }
#endif

	return 0;
}

/**
 * Write val to the specific gpio(by pin number).
 *
 * @param pin - pin number
 * @param val - HIGH or LOW
 * @return 0 if success, 1 if failed.
 */
uint32_t digitalWrite(uint16_t pin, uint8_t val)
{
	uint32_t msg=0;
	int i;
	int wait_cnt=0;
    volatile uint32_t *cureg;
    uint16_t port_no ;
	uint16_t index ;
	pthread_t	tid = pthread_self();

	if (!vertify_val(val)) {
		sdkerr("\nIllegal parameter : val=%u\n",val);
		return 1;
	}

	if (!vertify_pin(pin)) {
		sdkerr("\nIllegal parameter : pin=%u\n",pin);
		return 1;
	}
	
	/* check pwm pin enable or disable */
	for(i=0; i<PIN_PWM_MAX; i++) {
		/* if pwm of this pin is running, then disable it */
		if(pin_pwm_infos[i].state== PIN_PWM_USED
			&& pin_pwm_infos[i].pin_no != PIN_NONE
			&& pin_pwm_infos[i].pin_no == pin 
			&& tid != pin_pwm_infos[i].tid
			&& pin_pwm_infos[i].tid != 0) {
			if(pthread_cancel(pin_pwm_infos[i].tid)){
				sdkerr("\ndigitalWrite:pthread_cancel(pin_pwm_infos[%d].tid = %lu) failed !\n",
					i,pin_pwm_infos[i].tid);
				return 1;
			}
			while(pin_pwm_infos[i].state == PIN_PWM_USED && wait_cnt < 20) {
				wait_cnt ++;
				usleep(5);
			}
			break;
		}
	}
	
	if(!gpio_mmap()) {
		sdkerr("\ngpio_mmap failed!\n");
		return 1;
	}
	
	
    /* write register data */
#if defined(BOARD_ITEADUINO_PLUS) || defined(BOARD_ITEAD_CORE_EVB)
    /* get port_no and index by pin_no */
	port_no = pnp[pin].port_no;
	index   = pnp[pin].index;
	
	/* find the register address need to operate */
	cureg = (volatile uint32_t *)((uint32_t)gpio_base 
		+ port_no*0x24 + 4*4);
	if( val == HIGH) {
		CUREG |= (1 << index);
	} else if (val == LOW) {
		CUREG &= ~(1 << index);
	}
#elif defined(BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    pin   = pnp[pin].index;
    if( val == HIGH) {
        cureg = gpio_base + GPIO_GPSET_OFFSET/4 + pin/32;
    } else {
        cureg = gpio_base + GPIO_GPCLR_OFFSET/4 + pin/32;
    }
    CUREG = 0x1 << (1*(pin%32));
#elif defined(BOARD_BEAGLEBONEBLACK)
    if (!check_export_gpio(pin)) {
       sdkerr("\nexport_gpio failed!\n");
       return 1;
    }
    if ( val == HIGH) {
        sprintf(gpio_string,DIRECTION_GPIO,"high",pin);
    } else if (val == LOW) {
        sprintf(gpio_string,DIRECTION_GPIO,"low",pin);
    }
    
    if (!do_shell(gpio_string)) {
        sdkerr("\nset gpio output level failed!\n");
    }    
#endif
	
	return 0;
}

/**
 * Read state of the specific gpio(by pin number).
 *
 * @param pin - pin number
 * @return HIGH or LOW if success, 2 if failed.
 */
uint32_t digitalRead(uint16_t pin)
{
	uint32_t ret;
	uint32_t msg=0;
	int wait_cnt=0;
	int i;
    volatile uint32_t *cureg;
    uint16_t port_no ;
	uint16_t index ;
	pthread_t	tid = pthread_self();
    FILE *f_gpio;
    int value_gpio;
	if (!vertify_pin(pin)) {
		sdkerr("\nIllegal parameter : pin=%u\n",pin);
		return 2;
	}
	
	/* check pwm pin enable or disable */
	for(i=0; i<PIN_PWM_MAX; i++) {
		/* if pwm of this pin is running, then disable it */
		if(pin_pwm_infos[i].state == PIN_PWM_USED
			&& pin_pwm_infos[i].pin_no != PIN_NONE
			&& pin_pwm_infos[i].pin_no == pin 
			&& tid != pin_pwm_infos[i].tid
			&& pin_pwm_infos[i].tid != 0) {
			
			if(pthread_cancel(pin_pwm_infos[i].tid)){
				sdkerr("\ndigitalWrite:pthread_cancel(pin_pwm_infos[%d].tid = %lu) failed !\n",
					i,pin_pwm_infos[i].tid);
				return 1;
			}
			while(pin_pwm_infos[i].state == PIN_PWM_USED && wait_cnt < 20) {
				wait_cnt ++;
				usleep(5);
			}
			break;
		}
	}
	
	if(!gpio_mmap()) {
		sdkerr("\ngpio_mmap failed!\n");
		return 1;
	}

    /* read register data */
#if defined(BOARD_ITEADUINO_PLUS) || defined(BOARD_ITEAD_CORE_EVB)
    /* get port_no and index by pin_no */
	port_no = pnp[pin].port_no;
	index   = pnp[pin].index;
	
	/* find the register address need to operate */
	cureg = (volatile uint32_t *)((uint32_t)gpio_base 
		+ port_no*0x24 + 4*4);

	return (CUREG & (1<<index)) ? HIGH : LOW;
#elif defined(BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    pin   = pnp[pin].index;
    cureg = gpio_base + GPIO_GPLEV_OFFSET/4 + pin/32;
    return (CUREG & (0x1<<(pin%32))) ? HIGH : LOW;
#elif defined(BOARD_BEAGLEBONEBLACK)
    if (!check_export_gpio(pin)) {
       sdkerr("\nexport_gpio failed!\n");
       return 1;
    }
    sprintf(gpio_string,VALUE_GPIO,pin);
    f_gpio = fopen(gpio_string,"r");
    if(f_gpio == NULL) {
        sdkerr("\nopen gpio value file failed!\n");
    }
    value_gpio = fgetc(f_gpio);
    fclose(f_gpio);
    if(value_gpio == '1') {
        return HIGH;
    } else if(value_gpio == '0') {
        return LOW;
    }
#endif
}


/**
 * Set every pin of the bus to the mode specified
 *
 * @param bus - pointer to a bus which has been init
 * @param DB0 - bit0 of the bus
 * @param DB1 - bit1 of the bus
 * @param DB2 - bit2 of the bus
 * @param DB3 - bit3 of the bus
 * @param DB4 - bit4 of the bus
 * @param DB5 - bit5 of the bus
 * @param DB6 - bit6 of the bus
 * @param DB7 - bit7 of the bus
 * @param mode - INPUT or OUTPUT
 * @return 0 if success, 1 if failed.
 */
uint32_t set8BitsBUS(STRUCT_8BITS_BUS *bus,
	uint16_t DB0, uint16_t DB1, uint16_t DB2, uint16_t DB3,
	uint16_t DB4, uint16_t DB5, uint16_t DB6, uint16_t DB7,
	uint8_t mode)
{
	int index;
	uint32_t *busp = (uint32_t *)bus;
	if(busp == NULL) {
		sdkerr("\nset8BitsBUS: null pointer.\n");
		return 1;
	}
	
	busp[0] = DB0;
	busp[1] = DB1;
	busp[2] = DB2;
	busp[3] = DB3;
	busp[4] = DB4;
	busp[5] = DB5;
	busp[6] = DB6;
	busp[7] = DB7;
	
	for(index = 0; index < 8; index ++) {
		pinMode(busp[index],mode);
	}
	return 0;
}

/**
 * Set every pin of the bus to the mode specified. 
 *
 * @param bus - pointer to a bus which has been init
 * @param DB0 - bit0 of the bus
 * @param DB1 - bit1 of the bus
 * @param DB2 - bit2 of the bus
 * @param DB3 - bit3 of the bus
 * @param DB4 - bit4 of the bus
 * @param DB5 - bit5 of the bus
 * @param DB6 - bit6 of the bus
 * @param DB7 - bit7 of the bus
 * @param DB8 - bit8 of the bus
 * @param DB9 - bit9 of the bus
 * @param DB10 - bit10 of the bus
 * @param DB11 - bit11 of the bus
 * @param DB12 - bit12 of the bus
 * @param DB13 - bit13 of the bus
 * @param DB14 - bit14 of the bus
 * @param DB15 - bit15 of the bus
 * @param mode - INPUT or OUTPUT
 * @return 0 if success, 1 if failed.
 */
uint32_t set16BitsBUS(STRUCT_16BITS_BUS *bus, 
	uint16_t DB0, uint16_t DB1, uint16_t DB2, uint16_t DB3,
	uint16_t DB4, uint16_t DB5, uint16_t DB6, uint16_t DB7,
	uint16_t DB8, uint16_t DB9, uint16_t DB10,uint16_t DB11,
	uint16_t DB12,uint16_t DB13,uint16_t DB14,uint16_t DB15,
	uint8_t mode)
{
	int index;
	uint32_t *busp = (uint32_t *)bus;
	if(busp == NULL) {
		sdkerr("\nset16BitsBUS: null pointer.\n");
		return 1;
	}
	busp[0] = DB0;
	busp[1] = DB1;
	busp[2] = DB2;
	busp[3] = DB3;
	busp[4] = DB4;
	busp[5] = DB5;
	busp[6] = DB6;
	busp[7] = DB7;
	busp[8 ] = DB8 ;
	busp[9 ] = DB9 ;
	busp[10] = DB10;
	busp[11] = DB11;
	busp[12] = DB12;
	busp[13] = DB13;
	busp[14] = DB14;
	busp[15] = DB15;
	
	for(index = 0; index < 16; index ++) {
		pinMode(busp[index],mode);
	}

	return 0;
}

/**
 * Write val to bus.
 *
 * @param bus - pointer to a bus which has been set
 * @param val - the value with type of uint8_t
 * @return 0 if success, 1 if failed.
 */
uint32_t digitalWrite8(STRUCT_8BITS_BUS *bus, uint8_t val)
{
	uint32_t *busp = (uint32_t *)bus;
	int i;
	if ( busp == NULL ) {
		sdkerr("\ndigitalWrite8: null pointer.\n");
		return 1;
	}
	for(i=0;i<8;i++) {
		digitalWrite(busp[i],(val>>i)&(0x1));
	}
	return 0;
}

/**
 * Write val to bus.
 *
 * @param bus - pointer to a bus which has been set
 * @param val - the value with type of uint16_t
 * @return  0 if success, 1 if failed.
 */
uint32_t digitalWrite16(STRUCT_16BITS_BUS *bus, uint16_t val)
{
	uint32_t *busp = (uint32_t *)bus;
	int i;
	if ( busp == NULL ) {
		sdkerr("\ndigitalWrite16: null pointer.\n");
		return 1;
	}
	for(i=0;i<16;i++) {
		digitalWrite(busp[i],(val>>i)&(0x1));
	}
	return 0;
}

/**
 * Read value from bus. 
 *
 * @param bus - pointer to a bus which has been set
 * @return value of bus
 */
uint8_t digitalRead8(STRUCT_8BITS_BUS *bus)
{
	uint8_t ret = 0;
	uint32_t *busp = (uint32_t *)bus;
	int i;
	if ( busp == NULL ) {
		sdkerr("\ndigitalRead8: null pointer.\n");
		return 1;
	}
	for(i=0;i<8;i++) {
		if( digitalRead(busp[i]) == HIGH) {
			ret |= (0x1 << i);
		} else {
			ret &= ~(0x1 << i);
		}
	}
	return ret;
}

/**
 * Read value from bus. 
 *
 * @param bus - pointer to a bus which has been set
 * @return value of bus
 */
uint16_t digitalRead16(STRUCT_16BITS_BUS *bus)
{
	uint16_t ret = 0;
	uint32_t *busp = (uint32_t *)bus;
	int i;
	if ( busp == NULL ) {
		sdkerr("\ndigitalRead16: null pointer.\n");
		return 1;
	}
	for(i=0;i<16;i++) {
		if( digitalRead(busp[i]) == HIGH) {
			ret |= (0x1 << i);
		} else {
			ret &= ~(0x1 << i);
		}
	}
	return ret;
}

/*
 * @name	: pwm_run_clean
 * @desc	: recycle the space of pin_pwm_infos[index].
 * @param	: arg - index of thread to kill.
 * @return	: none.
 */
static void *pwm_run_clean(void *arg)
{
	uint32_t index = (uint32_t)arg;
	
    debug("pwm_run_clean :index = %d ",index);
	debug("\n	thread tid= %lu exit\n",pin_pwm_infos[index].tid);
	
	pin_pwm_infos[index].pin_no = PIN_NONE;
	pin_pwm_infos[index].duty = 0;
	pin_pwm_infos[index].tid = 0;
	pin_pwm_infos[index].high_us = 0;
	pin_pwm_infos[index].low_us = 0;
	pin_pwm_infos[index].state= PIN_PWM_FREE;
	
    return (void *)0;
}

/*
 * @name	: pwm_run
 * @desc	: thread to output pwm.
 * @param	: arg - index of thread in pin_pwm_infos.
 * @return	: (void *)0.
 */
static void *pwm_run(void *arg)
{
	uint32_t index = (uint32_t)arg;
	
	pthread_cleanup_push((void*)pwm_run_clean,(void *)index);
	
	pin_pwm_infos[index].tid = pthread_self();
	
	pinMode(pin_pwm_infos[index].pin_no,OUTPUT);
	
	for(;;) {
		if(pin_pwm_infos[index].duty == 0) {
			digitalWrite(pin_pwm_infos[index].pin_no,LOW);
			pthread_exit((void *)0);
		} else if( pin_pwm_infos[index].duty == (PIN_PWM_BASE_CNT) ) {
			digitalWrite(pin_pwm_infos[index].pin_no,HIGH);
			pthread_exit((void *)0);
		} else {
			digitalWrite(pin_pwm_infos[index].pin_no,HIGH);
			delayMicroseconds(pin_pwm_infos[index].high_us);
			digitalWrite(pin_pwm_infos[index].pin_no,LOW);
			delayMicroseconds(pin_pwm_infos[index].low_us);
		}
	}
	
    pthread_cleanup_pop(0);
}

/**
 * Output pwm with duty via the pin. 
 * 
 * @param pin - the number of the pin to output pwm.
 * @param duty - 0-255.
 * @return 0 - success, 1 - failed.
 * @note May be unstable when the duty be close to 0 or 255.
 *			  A delay should be insert between two analogWrite calls.
 */
uint32_t analogWrite(uint16_t pin, uint8_t duty)
{
	pthread_t  tid;
	uint32_t index = 0;
	int ret;
	int i;
	int pin_exist = 0;
	float rate;

	if(!gpio_mmap()) {
		sdkerr("\ngpio_mmap failed!\n");
		return 1;
	}
	
	if (!vertify_pin(pin)) {
		sdkerr("\nanalogWrite:Illegal parameter : pin=%d\n",pin);
		return 1;
	}
	
	for(i=0;i<PIN_PWM_MAX;i++) {
		if(pin_pwm_infos[i].state == PIN_PWM_USED 
			&& pin_pwm_infos[i].pin_no == pin) {
			index = i;
			pin_exist = 1;
			break;
		}
	}
	/* if not exist , new pin for pwm */
	if( !pin_exist) {
		
		for(i=0;i<PIN_PWM_MAX;i++){
			if(pin_pwm_infos[i].state == PIN_PWM_FREE){
				index = i;
				break;
			}
		}
		if (i==PIN_PWM_MAX) {
			sdkerr("\nanalogWrite failed ! too many pwm pins.\n");
			return 1;
		}
		
		debug("\nnew pwm pin\n");
		pin_pwm_infos[index].state= PIN_PWM_USED;
		pin_pwm_infos[index].duty = duty;
		pin_pwm_infos[index].pin_no = pin;
		rate = (duty*1.0/PIN_PWM_BASE_CNT);
		pin_pwm_infos[index].high_us = PWM_PERIOD_US * rate;
		pin_pwm_infos[index].low_us = PWM_PERIOD_US - pin_pwm_infos[index].high_us;
		
		ret = pthread_create(&tid, NULL, pwm_run, (void *)(index));
		if(ret) {
			sdkerr("\npthread_create failed !\n");
			return 1;
		}
		debug("\ntid=%lu , pin=%d, duty=%d (%.2f)\n", 
			tid,
			pin_pwm_infos[index].pin_no,
			pin_pwm_infos[index].duty,
			rate);
		usleep(100);
	} 
	else {
		debug("\nmodify pwm pin of duty\n");
		pin_pwm_infos[index].duty = duty;
		rate = (duty*1.0/PIN_PWM_BASE_CNT);
		pin_pwm_infos[index].high_us = PWM_PERIOD_US * rate;
		pin_pwm_infos[index].low_us = PWM_PERIOD_US - pin_pwm_infos[index].high_us;
		
		debug("\ntid=%lu , pin=%d, duty=%d (%.2f)\n", 
			pin_pwm_infos[index].tid,
			pin_pwm_infos[index].pin_no,
			pin_pwm_infos[index].duty,
			rate);
	}
	
	return 0;
}

