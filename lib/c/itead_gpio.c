/********************************************************************
* File		:	itead_gpio.c
* Desc		:	Provide the implementation of Arduino API on gpio
* Author	: 	Wu Pengfei
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/3/11
* History	:	Version		Modified by		Date		What
*				v1.0		Wu Pengfei		2014/3/11	Create
********************************************************************/

/*
 * (C) Copyright 2014
 * ITEAD Intelligent Systems Co., Ltd. <http://imall.iteadstudio.com>
 * Wu Pengfei<wupangfee@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/ioctl.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <itead_gpio.h>
#include <itead_delay.h>
#include <itead_gpio_pin_map.h>
#include <itead_global.h>

//#define DEBUG
#include <itead_debug.h>

typedef struct PIN_PWM_INFO_ST{
	uint16_t 	pin_no;
	uint8_t 	duty;
	pthread_t	tid;
	uint8_t		state;
	uint32_t	high_us;
	uint32_t	low_us;
} PIN_PWM_INFO;

/*
 * PWM_FREQUENCY 	- pwm frequency.
 * PWM_PERIOD_US 	- pwm period in microsecond.
 * PIN_PWM_BASE_CNT - duty base number.
 * PIN_PWM_MAX 		- maxmial threads for pwm output. 
 */
#define PWM_FREQUENCY		(1000*1)		// 1kHz
#define PWM_PERIOD_US		((1.0/PWM_FREQUENCY)*(1000000.0))
#define PIN_PWM_BASE_CNT	255
#define PIN_PWM_MAX			8

#define PIN_PWM_USED		1
#define PIN_PWM_FREE		0

#define CUREG   (*(cureg))

static PIN_PWM_INFO pin_pwm_infos[PIN_PWM_MAX];
static int fd = -1;
static volatile uint32_t *gpio_base;


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
	if (fd >= 0) {
		return 1;
	}
    if ((fd = open ("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) == -1) {
        printf("\nopen /dev/mem error !\n");
        return 0;
    }

#if defined(BOARD_ITEADUINO_PLUS)
     if ((gpio_base = (volatile uint32_t *)mmap(0, 
            GPIO_SIZE, 
            PROT_READ|PROT_WRITE, 
            MAP_SHARED,
            fd, 
            GPIO_BASE-0x800)) == MAP_FAILED){
        printf("\nmmap error !\n");
        return 0;
    }
    gpio_base += 0x800/4;
#elif defined(BOARD_RASPBERRY_RV2)
    if ((gpio_base = (volatile uint32_t *)mmap(0, 
            GPIO_SIZE, 
            PROT_READ|PROT_WRITE, 
            MAP_SHARED,
            fd, 
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

/*
 * @name	: pinMode
 * @desc	: set the specific gpio(by pin number) to related mode.
 * @param	: pin - pin number.
 *			  mode - INPUT or OUTPUT.
 * @return	: 0 if success, 1 if failed.
 */
uint32_t pinMode(uint16_t pin, uint8_t mode)
{
	int fd;
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
#ifdef BOARD_ITEADUINO_PLUS
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

#elif defined(BOARD_RASPBERRY_RV2)
    /* config pin as OUTPUT or INPUT */
   pin   = pnp[pin].index;
   cureg = gpio_base + GPIO_GPFSEL_OFFSET/4 + pin/10;
   CUREG &= ~(0x7 << (3*(pin%10)));
   if(mode == INPUT) {
       CUREG |=  (0x0 << (3*(pin%10)));
   } else {
       CUREG |=  (0x1 << (3*(pin%10)));
   }

#endif

	return 0;
}

/*
 * @name	: digitalWrite
 * @desc	: write val to the specific gpio(by pin number).
 * @param	: pin - pin number
 *			  val - HIGH or LOW
 * @return	: 0 if success, 1 if failed.
 */
uint32_t digitalWrite(uint16_t pin, uint8_t val)
{
	int fd;
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
#ifdef BOARD_ITEADUINO_PLUS
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
#elif defined(BOARD_RASPBERRY_RV2)
    pin   = pnp[pin].index;
    if( val == HIGH) {
        cureg = gpio_base + GPIO_GPSET_OFFSET/4 + pin/32;
    } else {
        cureg = gpio_base + GPIO_GPCLR_OFFSET/4 + pin/32;
    }
    CUREG = 0x1 << (1*(pin%32)); 

#endif
	
	return 0;
}

/*
 * @name	: digitalRead
 * @desc	: read state of the specific gpio(by pin number).
 * @param	: pin - pin number
 * @return	: HIGH or LOW if success, 2 if failed.
 */
uint32_t digitalRead(uint16_t pin)
{
	int fd;
	uint32_t ret;
	uint32_t msg=0;
	debug("\ndigitalRead begin\n");
	int wait_cnt=0;
	int i;
    volatile uint32_t *cureg;
    uint16_t port_no ;
	uint16_t index ;
	pthread_t	tid = pthread_self();

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
#ifdef BOARD_ITEADUINO_PLUS
    /* get port_no and index by pin_no */
	port_no = pnp[pin].port_no;
	index   = pnp[pin].index;
	
	/* find the register address need to operate */
	cureg = (volatile uint32_t *)((uint32_t)gpio_base 
		+ port_no*0x24 + 4*4);

	return (CUREG & (1<<index)) ? HIGH : LOW;
#elif defined(BOARD_RASPBERRY_RV2)
    pin   = pnp[pin].index;
    cureg = gpio_base + GPIO_GPLEV_OFFSET/4 + pin/32;
    return (CUREG & (0x1<<(pin%32))) ? HIGH : LOW;
#endif
}


/*
 * @name	: set8BitsBUS
 * @desc	: set every pin of the bus to the mode specified
 * @param	: bus - pointer to a bus which has been init
 *			  DBn - n=0-7, the corresponding bits of bus
 *			  mode - INPUT or OUTPUT
 * @return	: 0 if success, 1 if failed.
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

/*
 * @name	: set16BitsBUS
 * @desc	: set every pin of the bus to the mode specified
 * @param	: bus - pointer to a bus which has been init
 *			  DBn - n=0-15, the corresponding bits of bus
 *			  mode - INPUT or OUTPUT
 * @return	: 0 if success, 1 if failed.
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

/*
 * @name	: digitalWrite8
 * @desc	: write val to bus
 * @param	: bus - pointer to a bus which has been set
 *			  val - the value with type of uint8_t
 * @return	: 0 if success, 1 if failed.
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

/*
 * @name	: digitalWrite16
 * @desc	: write val to bus
 * @param	: bus - pointer to a bus which has been set
 *			  val - the value with type of uint16_t
 * @return	: 0 if success, 1 if failed.
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

/*
 * @name	: digitalRead8
 * @desc	: read value from bus
 * @param	: bus - pointer to a bus which has been set
 * @return	: value of bus
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

/*
 * @name	: digitalRead16
 * @desc	: read value from bus
 * @param	: bus - pointer to a bus which has been set
 * @return	: value of bus
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

/*
 * @name	: analogWrite
 * @desc	: output pwm with duty via the pin.
 * @param	: pin - the number of the pin neet to output pwm.
 *			  duty ratio - 0-255.
 * @return	: 0 - success, 1 - failed.
 * @notes	: may be unstable when the duty be close to 0 or 255.
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

