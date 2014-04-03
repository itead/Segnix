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

#include <itead_gpio.h>
#include <itead_delay.h>
#include <itead_gpio_pin_map.h>
#include <itead_global.h>

//#define DEBUG
#include <itead_debug.h>



#define DIGITAL_GPIO_FILE			"/dev/itead_a10_a20_core_gpio"
#define DIGITAL_GPIO_MODULE			"itead_a10_a20_core_gpio"

/*
 * ioctl command 
 */
#define MEMDEV_IOC_MAGIC  	'k'
#define PIN_MODE_CMD   		_IOWR(MEMDEV_IOC_MAGIC, 1, uint32_t)
#define DIGITAL_READ_CMD 	_IOWR(MEMDEV_IOC_MAGIC, 2, uint32_t)
#define DIGITAL_WRITE_CMD 	_IOWR(MEMDEV_IOC_MAGIC, 3, uint32_t)


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
#define PWM_FREQUENCY		(1000)		// 1kHz
#define PWM_PERIOD_US		((1.0/PWM_FREQUENCY)*(1000000.0))
#define PIN_PWM_BASE_CNT	255
#define PIN_PWM_MAX			8

#define PIN_PWM_USED		1
#define PIN_PWM_FREE		0

static PIN_PWM_INFO pin_pwm_infos[PIN_PWM_MAX];
static int fd = -1;

/*
 * ioctl msg format:
 * uint32_t msg;
 * msg[7:0] : data,HIGH/LOW,INPUT/OUTPUT
 * msg[23:8]: pin number
 * msg[31:24]: reserved
 */

/*
 * @name	: open_gpio_dev
 * @desc	: open the node of gpio driver.
 * @param	: 
 * @return	: fd - file descriptor of DIGITAL_GPIO_FILE if success.
 *			  -1 - if open DIGITAL_GPIO_FILE failed.
 */
static int32_t open_gpio_dev(void)
{
	char modstr[50];
	
	/* open already */
	if (fd >= 0) {
		return fd;
	}

	/* not open */
	fd = open(DIGITAL_GPIO_FILE,O_RDWR);
	if(fd<0) {
		strcpy(modstr,"modprobe ");
		strcat(modstr,DIGITAL_GPIO_MODULE);
		if(system(modstr) == -1) {
			sdkerr("\nCommand: %s failed !!!\n",modstr);
			return -1;
		}
		sleep(1);
	} else {
		return fd;
	}
	
	fd = open(DIGITAL_GPIO_FILE,O_RDWR);
	if(fd < 0) {
		sdkerr("\nopen file %s failed !!!\n",DIGITAL_GPIO_FILE);
		return fd;
	} else {
		return fd;
	}
}

/*
 * @name	: vertify_pin
 * @desc	: vertify pin for availability.
 * @param	: pin - pin number.
 * @return	: 1 if available, 0 if unavailable.
 */
static uint32_t vertify_pin(uint16_t pin)
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
static uint32_t vertify_mode(uint8_t mode)
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
static uint32_t vertify_val(uint8_t val)
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
	
	debug("\npinMode begin\n");
	
	if (!vertify_mode(mode)) {
		sdkerr("\nIllegal parameter : mode=%u\n",mode);
		return 1;
	}

	if (!vertify_pin(pin)) {
		sdkerr("\nIllegal parameter : pin=%u\n",pin);
		return 1;
	}
	
	if((fd = open_gpio_dev()) == -1) {
		sdkerr("\nopen file failed!\n");
		return 1;
	}
	
	/* create msg */
	msg = (pin<<8) | mode;

	/* send msg to kernel driver */
	if(ioctl(fd,PIN_MODE_CMD,&msg)) {
		sdkerr("\nioctl PIN_MODE_CMD failed!\n");
		return 1;
	}
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
	
	if((fd = open_gpio_dev()) == -1) {
		sdkerr("\nopen file failed!\n");
		return 1;
	}
	
	/* create msg */
	msg = (pin<<8) | val;

	/* send msg to kernel driver */
	if(ioctl(fd,DIGITAL_WRITE_CMD, &msg)) {
		sdkerr("\nioctl DIGITAL_WRITE_CMD failed!\n");
		return 1;
	}
	
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
	
	if((fd = open_gpio_dev()) == -1) {

		sdkerr("\nopen file failed!\n");
		return 2;
	}
	
	/* create msg */
	msg =  (pin<<8);
	
	/* send msg to kernel driver */
	/* returned value saves in msg */
	if(ioctl(fd,DIGITAL_READ_CMD, &msg)) {
		sdkerr("\nioctl DIGITAL_READ_CMD failed!\n");
		return 2;
	}
	
	debug("ret msg = 0x%X\n",msg);
	
	return (msg==0) ? LOW : HIGH;
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

#if 0 /* no call */
static int set_high_pri (int pri)
{
	//nice(pri);
	 struct sched_param param;  
       
        int maxRR;  
        
        maxRR = sched_get_priority_max(SCHED_RR);  
        if( maxRR == -1){  
            perror("sched_get_priority_max() error!\n");  
            exit(1);  
        }  

        param.sched_priority = maxRR;  
        if(sched_setscheduler(getpid(), SCHED_RR, &param) == -1){  
            perror("sched_setscheduler() error!\n");  
            exit(1);  
        }
		sdkerr ("Policy %d\n", sched_getscheduler(0));  
	return 0;
}
#endif

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
		if(pin_pwm_infos[index].duty <= 5) {
			digitalWrite(pin_pwm_infos[index].pin_no,LOW);
			pthread_exit((void *)0);
		} else if( pin_pwm_infos[index].duty >= (PIN_PWM_BASE_CNT-5) ) {
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

	if((fd = open_gpio_dev()) == -1) {
		sdkerr("\nopen file failed!\n");
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

