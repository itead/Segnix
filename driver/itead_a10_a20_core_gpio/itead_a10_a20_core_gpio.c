/********************************************************************
* File		:	itead_a10_a20_core_gpio.c
* Desc		:	ioctl to manipulate gpio itead a10/a20 core gpio 
*				registers.
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/ioctl.h>

#include "itead_a10_a20_core_gpio.h"

//#define DEBUG

#ifdef DEBUG
#define _DEBUG	1
#else
#define _DEBUG	0
#endif

#define debug(fmt, args...)		\
		if (_DEBUG) { \
			printk("\n[kernel error:%s,%d,%s]-> ",__FILE__,__LINE__,__FUNCTION__);\
			printk(fmt, ##args);	\
		}

/*
 * Global variables in file scope are here.
 */
static int32_t major;
static struct class *digital_gpio_driver_class;
static struct device *digital_gpio_driver_dev;
static volatile uint32_t *cureg = NULL;
static volatile uint32_t *a10_pio_base = NULL;
static uint16_t port_no;
static uint16_t index;
static DEFINE_MUTEX(gpio_lock);

/*
 * @name	: vertify_pin
 * @desc	: vertify the availability of pin
 * @param	: pin - the number of specific pin.
 * @return	: 1 if available, 0 if unavailable.
 */
static uint32_t vertify_pin(uint16_t pin)
{
	if (pnp[pin].port_no != PORT_NONE 
		&& pnp[pin].index != INDEX_NONE) {
		return 1;
	} else {
		return 0;
	}
}

/*
 * @name	: pin_mode
 * @desc	: config pin to the specific mode.
 * @param	: pin - the number of specific pin.
 *			  mode - INPUT or OUTPUT.
 * @return	: none.
 */
static void pin_mode(uint16_t pin, uint8_t mode) 
{

	debug("ping_mode(%u,%u) begin\n",pin,mode);
	/* get port_no and index by pin_no */
	port_no = pnp[pin].port_no;
	index   = pnp[pin].index;
	
	/* find the register address need to operate */
	cureg = (volatile uint32_t *)((uint32_t)a10_pio_base 
		+ port_no*0x24 + 4*(index/8));
	
	if( mode == INPUT ) {
		CUREG &= ~(0xF<<((index%8)*4));
		CUREG |= A10_GPIO_INPUT << ((index%8)*4);
	} else if(mode == OUTPUT) {
		CUREG &= ~(0xF<<((index%8)*4));
		CUREG |= A10_GPIO_OUTPUT << ((index%8)*4);
	} else {
		printk("\nIllegal mode\n");
	}
	
}

/*
 * @name	: digital_write
 * @desc	: write val to the bit of data register corresponding to the pin.
 * @param	: pin - the number of specific pin.
 *			  val - HIGH or LOW.
 * @return	: none.
 */
static void digital_write(uint16_t pin, uint8_t val) 
{
	debug("digital_write(%u,%u) begin\n",pin,val);
	/* get port_no and index by pin_no */
	port_no = pnp[pin].port_no;
	index   = pnp[pin].index;
	
	/* find the register address need to operate */
	cureg = (volatile uint32_t *)((uint32_t)a10_pio_base 
		+ port_no*0x24 + 4*4);
	if( val == HIGH) {
		CUREG |= (1 << index);
	} else if (val == LOW) {
		CUREG &= ~(1 << index);
	} else {
		printk("\nIllegal val\n");
	}
}

/*
 * @name	: digital_read
 * @desc	: read val from the bit of data register corresponding to the pin.
 * @param	: pin - the number of specific pin.
 * @return	: HIGH or LOW.
 */
static uint32_t digital_read(uint16_t pin) {
	debug("digital_read(%u) begin\n",pin);
	/* get port_no and index by pin_no */
	port_no = pnp[pin].port_no;
	index   = pnp[pin].index;
	
	/* find the register address need to operate */
	cureg = (volatile uint32_t *)((uint32_t)a10_pio_base 
		+ port_no*0x24 + 4*4);
	
	return (CUREG & (1<<index)) ? HIGH : LOW;
}

static int digital_gpio_open (struct inode * inode, struct file * file)
{	
	/* Only one task can get the lock of gpio_lock */
	if(!mutex_trylock(&gpio_lock)) {
		printk("\nfile:/dev/%s is busy now !\n",DIGITAL_GPIO_NODE);
		return -1;
	}
	debug("digital_gpio_open done\n");
	return 0;
}

static int digital_gpio_release (struct inode *node, struct file *file)
{
	/* Unlock gpio_lock after access */
	mutex_unlock(&gpio_lock);
	debug("\ndigital_gpio_release end\n");
	return 0;
}

static long digital_gpio_ioctl (struct file *filp, 
	unsigned int cmd, 
	unsigned long msgp) {
	
	long ret=0;
	uint32_t msg=0;
	uint32_t readpin=0xbad;
	uint16_t pin=0;
	uint8_t data=0;
	
	debug("digital_gpio_ioctl begin\n");
	
	if(copy_from_user((void *)&msg, 
		(const void __user * )msgp, 
		sizeof(msg))){
		debug("copy_from_user failed !!!\n");
		return -EFAULT;
	}
	
	debug("msg = 0x%X\n",msg);
	
	pin = (msg&0x00FFFF00)>>8;
	data = msg&0x000000FF;
	
	if(!vertify_pin(pin)){
		printk("\nIllegal parameter : pin=%d\n",pin);
		return -EFAULT;
	}
	
	switch(cmd) {
	case PIN_MODE_CMD:
		debug("PIN_MODE_CMD ok\n");
		pin_mode(pin, data);
		break;
		
	case DIGITAL_READ_CMD:
		debug("DIGITAL_READ_CMD ok\n");
		readpin = digital_read(pin);
		if(copy_to_user((void __user*)msgp, 
			(const void * )(&readpin), 
			sizeof(readpin))){
			debug("copy_from_user failed !!!\n");
			return -EFAULT;
		}
		break;
		
	case DIGITAL_WRITE_CMD:
		debug("DIGITAL_WRITE_CMD ok\n");
		digital_write(pin, data);
		break;
		
	default:
		debug("\nIllegal parameter cmd\n");
		return -EINVAL;
	}

	debug("digital_gpio_ioctl end\n");
	return ret;
}


static struct file_operations digital_gpio_fops = {
	.owner			= 	THIS_MODULE,
    .open       	=	digital_gpio_open,
    .unlocked_ioctl	=	digital_gpio_ioctl,
    .release		= 	digital_gpio_release,
};

static int  digital_gpio_driver_init(void)
{
	major = register_chrdev(0, "digital_gpio_device",&digital_gpio_fops);
	digital_gpio_driver_class = class_create(THIS_MODULE, 
		(const char *)("digital_gpio_driver_class"));
	
	digital_gpio_driver_dev = device_create(digital_gpio_driver_class, 
		(struct device *)NULL, 
		MKDEV(major, 0), 
		(void *)NULL, 
		(const char *)DIGITAL_GPIO_NODE);
	
	a10_pio_base = (volatile uint32_t *)ioremap(A10_PIO_BASE, 360);
	
	debug("digital_gpio_driver_init done\n");
	
	return 0;

}

static void digital_gpio_driver_exit(void)
{
	iounmap(a10_pio_base);
	device_destroy(digital_gpio_driver_class,MKDEV(major, 0));
	class_destroy(digital_gpio_driver_class);
	unregister_chrdev(major,"digital_gpio_device");
	debug("digital_gpio_driver_exit done\n");
}

module_init(digital_gpio_driver_init);
module_exit(digital_gpio_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wu Pengfei");
MODULE_DESCRIPTION("itead a10/a20 core gpio driver");
MODULE_VERSION("V1.0");
