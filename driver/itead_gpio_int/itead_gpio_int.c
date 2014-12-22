/********************************************************************
* File		:	itead_gpio_int.c
* Desc		:	ioctl to manipulate gpio external interrupts of itead a10/a20 core
* Author	: 	Wu Pengfei
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/8/14
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
#include <linux/poll.h>
#include <linux/threads.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/time.h>
#include <linux/signal.h>

#ifndef DEBUG
#define DEBUG
#endif

#ifdef DEBUG
#define debug(fmt, args...)		                                                    \
    do {                                                                            \
			printk("\n[DEBUG:%s,%d,%s]-> ",__FILE__,__LINE__,__FUNCTION__);  \
			printk(fmt, ##args);	                                                \
    } while(0)
#else
#define debug(fmt, args...) do {} while(0)
#endif


#define GPIO_EINT_NUMBER_TOTAL      (32)

typedef enum GpioEintMode{
    GPIO_EINT_CFG_RAISING   = 0x0,
    GPIO_EINT_CFG_FALLING   = 0x1,
    GPIO_EINT_CFG_HIGH      = 0x2,
    GPIO_EINT_CFG_LOW       = 0x3,
    GPIO_EINT_CFG_DEDGE     = 0x4,
} GpioEintMode;

typedef struct gpio_eint_ioctl_t {
    uint16_t pin;
    GpioEintMode mode;
    uint32_t state;
} gpio_eint_ioctl_t;


#define MEMDEV_IOC_MAGIC  	        'k'
#define CMD_EINT_REQUEST            _IOW(MEMDEV_IOC_MAGIC, 4, gpio_eint_ioctl_t)
#define CMD_EINT_FREE               _IOW(MEMDEV_IOC_MAGIC, 5, gpio_eint_ioctl_t)
#define CMD_EINT_NO_INTERRUPT       _IO(MEMDEV_IOC_MAGIC, 6)
#define CMD_EINT_INTERRUPT          _IO(MEMDEV_IOC_MAGIC, 7)
#define CMD_EINT_GET_STATE          _IOR(MEMDEV_IOC_MAGIC, 8, gpio_eint_ioctl_t)

typedef struct GpioEintMap {
    uint16_t pin;
    uint32_t eint_no;
} GpioEintMap;


#define GPIO_PIN_NONE       (65535)
#define GPIO_EINT_NONE      (65535)

static const GpioEintMap gpio_eint_map[GPIO_EINT_NUMBER_TOTAL+1] = {
    /* pin, eint_no */
    { 109 , 22 },
    { 110, 23 },
    { 111, 24 },
    { 112, 25 },
    { 137, 26 },
    { 138, 27 },
    { 78, 28 },
    { 77, 29 },
    { 76, 30 },
    { 75, 31 },
    { GPIO_PIN_NONE, GPIO_EINT_NONE },
};

static inline uint32_t get_eint_no(uint16_t pin)
{
    int32_t i;
    
    for (i=0; i<(sizeof(gpio_eint_map)/sizeof(gpio_eint_map[0])); i++) {
        if (gpio_eint_map[i].pin        == pin
            && gpio_eint_map[i].pin     != GPIO_PIN_NONE
            && gpio_eint_map[i].eint_no != GPIO_EINT_NONE
        ) {
            return gpio_eint_map[i].eint_no;
        }

        if (gpio_eint_map[i].pin == GPIO_PIN_NONE) {
            return GPIO_EINT_NONE;
        }
    }

    return GPIO_EINT_NONE;
}

#define HIGH		                (0x1)
#define LOW			                (0x0)

#define INPUT 		                (0x0)
#define OUTPUT 		                (0x1)

#define GPIO_MODE_EINT 	            (0b110)
#define GPIO_MODE_INPUT 		    (0b000)
#define GPIO_MODE_OUTPUT 	        (0b001)


#define PORT_A	 	(0)
#define PORT_B		(1)
#define PORT_C		(2)
#define PORT_D		(3)
#define PORT_E		(4)
#define PORT_F	 	(5)
#define PORT_G		(6)
#define PORT_H		(7)
#define PORT_I		(8)

#define PORT_NONE	        (65535)
#define INDEX_NONE	        (65535)



typedef struct pin_no_port_index_map{
	uint16_t port_no;
	uint16_t index;
} pin_no_port_index_map;


typedef enum GpioEintRegistered {
    GPIO_EINT_FREE = 0,
    GPIO_EINT_REGISTERED,
} GpioEintRegistered;


#define ITEAD_GPIO_INT_NODE			("itead_gpio_int")

#define A10_A20_PIO_BASE		(0x01C20800)

#define PIO_INT_CFG0_OFFSET     (0x200)
#define PIO_INT_CFG1_OFFSET     (0x204)
#define PIO_INT_CFG2_OFFSET     (0x208)
#define PIO_INT_CFG3_OFFSET     (0x20C)
#define PIO_INT_CTL_OFFSET      (0x210)
#define PIO_INT_STA_OFFSET      (0x214)

#define PIO_INT_CFG0    (*((volatile uint32_t *)((uint32_t)a10_a20_pio_base+PIO_INT_CFG0_OFFSET)))    // PIO Interrrupt Configure Register 0
#define PIO_INT_CFG1    (*((volatile uint32_t *)((uint32_t)a10_a20_pio_base+PIO_INT_CFG1_OFFSET)))    // PIO Interrrupt Configure Register 1
#define PIO_INT_CFG2    (*((volatile uint32_t *)((uint32_t)a10_a20_pio_base+PIO_INT_CFG2_OFFSET)))    // PIO Interrrupt Configure Register 2
#define PIO_INT_CFG3    (*((volatile uint32_t *)((uint32_t)a10_a20_pio_base+PIO_INT_CFG3_OFFSET)))    // PIO Interrrupt Configure Register 3
#define PIO_INT_CTL     (*((volatile uint32_t *)((uint32_t)a10_a20_pio_base+PIO_INT_CTL_OFFSET)))    // PIO Interrupt Control Register
#define PIO_INT_STA     (*((volatile uint32_t *)((uint32_t)a10_a20_pio_base+PIO_INT_STA_OFFSET)))    // PIO Interrupt Status Register

#define GPIO_IRQ_NO     SW_INT_IRQNO_PIO


static const pin_no_port_index_map pnp[] = {
/* pin_no = port_no, index */
    [109] = {PORT_I, 10},
    [110] = {PORT_I, 11},
    [111] = {PORT_I, 12},
    [112] = {PORT_I, 13},
    [137] = {PORT_I, 14},
    [138] = {PORT_I, 15},
    [78 ] = {PORT_I, 16},
    [77 ] = {PORT_I, 17},
    [76 ] = {PORT_I, 18},
    [75 ] = {PORT_I, 19},
};



static DEFINE_MUTEX(gpio_lock);
static int32_t major;
static struct class *itead_gpio_int_class;
static struct device *itead_gpio_int_dev;
static uint16_t port_no;
static uint16_t index;
static volatile uint32_t *a10_a20_pio_base = NULL;
static volatile uint32_t *cureg = NULL;
static struct fasync_struct *gpio_async = NULL;
static volatile uint32_t gpio_eint_state = 0;
static GpioEintRegistered registered_eint[GPIO_EINT_NUMBER_TOTAL] = {0};
static uint8_t eint_old_mode[GPIO_EINT_NUMBER_TOTAL] = {0};



/*
 * @name	: vertify_pin
 * @desc	: vertify the availability of pin
 * @param	: pin - the number of specific pin.
 * @return	: 1 if available, 0 if unavailable.
 */
static uint32_t vertify_pin(uint16_t pin)
{
	if (pnp[pin].port_no != PORT_NONE 
		&& pnp[pin].index != INDEX_NONE) 
	{
		return 1;
	} else {
		return 0;
	}
}

/*
 * @name	: pin_mode
 * @desc	: config pin to the specific mode.
 * @param	: pin - the number of specific pin.
 *			  mode - INPUT or OUTPUT or GPIO_MODE_EINT or mode < 0x8
 * @return	: none.
 */
static void pin_mode(uint16_t pin, uint8_t mode) 
{
	debug("ping_mode(%u,%u) begin\n",pin,mode);
	/* get port_no and index by pin_no */
	port_no = pnp[pin].port_no;
	index   = pnp[pin].index;
	
	/* find the register address need to operate */
	cureg = (volatile uint32_t *)((uint32_t)a10_a20_pio_base 
		+ port_no*0x24 + 4*(index/8));
	
	if(mode == INPUT) {
		(*cureg) &= ~(0xF<<((index%8)*4));
		(*cureg) |= GPIO_MODE_INPUT << ((index%8)*4);
	} else if(mode == OUTPUT) {
		(*cureg) &= ~(0xF<<((index%8)*4));
		(*cureg) |= GPIO_MODE_OUTPUT << ((index%8)*4);
	} else if (mode == GPIO_MODE_EINT){
	    (*cureg) &= ~(0xF<<((index%8)*4));
		(*cureg) |= GPIO_MODE_EINT << ((index%8)*4);
	} else if (mode < 0x8) {
	    (*cureg) &= ~(0xF<<((index%8)*4));
	    (*cureg) |= mode << ((index%8)*4);
	} else {
		printk("\nIllegal mode\n");
	}
	
}

/*
 * @name	: get_pin_mode
 * @desc	: get the mode of pin.
 * @param	: pin - the number of specific pin.
 * @return	: Mode value (low 3 bits is valid).
 */
static uint8_t get_pin_mode(uint16_t pin) 
{
	/* get port_no and index by pin_no */
	port_no = pnp[pin].port_no;
	index   = pnp[pin].index;
	
	/* find the register address need to operate */
	cureg = (volatile uint32_t *)((uint32_t)a10_a20_pio_base 
		+ port_no*0x24 + 4*(index/8));

	return (uint8_t)(0b111 && ( (*cureg)>>( (index%8) * 4) ));	
}

static inline uint16_t get_pin_no(uint32_t eint_no)
{
    uint32_t i;
    for(i=0; i<ARRAY_SIZE(gpio_eint_map); i++){
        if(gpio_eint_map[i].eint_no == eint_no)
            return gpio_eint_map[i].pin;
    }
    return GPIO_PIN_NONE;
}

#if 0 
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
	cureg = (volatile uint32_t *)((uint32_t)a10_a20_pio_base 
		+ port_no*0x24 + 4*4);
	if( val == HIGH) {
		(*cureg) |= (0x1 << index);
	} else if (val == LOW) {
		(*cureg) &= ~(0x1 << index);
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
	cureg = (volatile uint32_t *)((uint32_t)a10_a20_pio_base 
		+ port_no*0x24 + 4*4);
	
	return ((*cureg) & (0x1<<index)) ? HIGH : LOW;
}
#endif

static inline int32_t vertify_eint_mode(GpioEintMode mode)
{
    if ((mode == GPIO_EINT_CFG_RAISING)
         || (mode == GPIO_EINT_CFG_FALLING)
         || (mode == GPIO_EINT_CFG_HIGH)
         || (mode == GPIO_EINT_CFG_LOW)
         || (mode == GPIO_EINT_CFG_DEDGE)
    ) {
        return 1;
    }
    return 0;
}



static inline int32_t gpio_eint_request(uint16_t pin, GpioEintMode mode)
{
    uint32_t eint_no;
    
    if (!vertify_pin(pin)){
        printk("Invalid pin=%d\n", pin);
        return -EINVAL;
    }
    
    if (!vertify_eint_mode(mode)) {
        printk("Invalid eint mode=%d\n", mode);
        return -EINVAL;
    }

    eint_no = get_eint_no(pin);
    if (eint_no == GPIO_EINT_NONE) {
        printk("Invalid parameters pin=%d, mode=%d\n", pin, mode);
        return -EINVAL;
    }

    /* Save old pin mode */
    eint_old_mode[eint_no] = get_pin_mode(pin);
    
    /* Set GPIO as interrupt function */
    pin_mode(pin, GPIO_MODE_EINT);
    
    /* Set trigger method */
    cureg = a10_a20_pio_base + (PIO_INT_CFG0_OFFSET/4) + eint_no/8;
    (*(cureg)) &= ~(0xF << ((eint_no % 8)*4));
    (*(cureg)) |=  ((uint32_t)mode << ((eint_no % 8)*4));

    /* Clear pending bit */
    PIO_INT_STA |= 0x1 << eint_no;

    /* Register */
    registered_eint[eint_no] = GPIO_EINT_REGISTERED;

    /* Enable */
    PIO_INT_CTL |= 0x1 << eint_no;

    debug("function done.\n");
    return 0;
}

static inline int32_t gpio_eint_free(uint16_t pin)
{
    uint32_t eint_no;

    if (!vertify_pin(pin)) {
        printk("Invalid pin=%d\n", pin);
        return -EINVAL;
    }
    
    eint_no = get_eint_no(pin);
    if (eint_no == GPIO_EINT_NONE) {
        printk("Invalid parameters pin=%d\n", pin);
        return -EINVAL;
    }

    /* Disable */
    PIO_INT_CTL &= ~(0x1 << eint_no);
    
    /* Clear pending bit */
    PIO_INT_STA |= 0x1 << eint_no;
    
    /* Unregister */
    registered_eint[eint_no] = GPIO_EINT_FREE;

    /* Restore pin mode saved before */
    pin_mode(pin, eint_old_mode[eint_no]);
    
    debug("function done.\n");
    return 0;
}

static inline int32_t gpio_eint_no_interrupt(void);
static inline void gpio_eint_free_all(void)
{
    uint32_t i;
    
    gpio_eint_no_interrupt();
    
    /* Disable EINTX registered and restore the pin mode saved before */
    for (i=0; i<ARRAY_SIZE(registered_eint); i++) {
        if (registered_eint[i] == GPIO_EINT_REGISTERED) {
            registered_eint[i] = GPIO_EINT_FREE;        /* Unregister */
            pin_mode(get_pin_no(i), eint_old_mode[i]);  /* Restore pin mode saved before */
        }
    }
}

static inline int32_t gpio_eint_no_interrupt(void)
{
    PIO_INT_CTL = 0x00000000;    /* Disable EINTX */
    PIO_INT_STA = 0xFFFFFFFF;    /* Clear EINTX Pending bit by writing 1 */
    return 0;
}

static inline int32_t gpio_eint_interrupt(void)
{
    int32_t i;
    
    PIO_INT_STA = 0xFFFFFFFF;    /* Clear EINTX Pending bit by writing 1 */
    
    /* Enable EINTX registered */
    for (i=0; i<ARRAY_SIZE(registered_eint); i++) {
        if (registered_eint[i] == GPIO_EINT_REGISTERED) {
            PIO_INT_CTL |= 0x1 << i;
        }
    }
    
    return 0;
}

static long digital_gpio_ioctl (struct file *filp, unsigned int cmd, unsigned long argp)
{
	int32_t ret = 0;
	gpio_eint_ioctl_t eint_data;
	
	debug("digital_gpio_ioctl begin\n");
	
	switch(cmd) {
	case CMD_EINT_REQUEST:
	    {   
	        if(copy_from_user((void *)&eint_data, (const void __user * )argp, sizeof(eint_data))) {
        		debug("copy_from_user failed !!!\n");
        		return -EFAULT;
        	}
        	ret = gpio_eint_request(eint_data.pin, eint_data.mode);
	        
	        break;
	    }
	case CMD_EINT_FREE:
	    {
	        if(copy_from_user((void *)&eint_data, (const void __user * )argp, sizeof(eint_data))) {
        		debug("copy_from_user failed !!!\n");
        		return -EFAULT;
        	}
            ret = gpio_eint_free(eint_data.pin);
	            
	        break;
	    }
	case CMD_EINT_NO_INTERRUPT:
	    {
            ret = gpio_eint_no_interrupt();
	        break;
	    }
	case CMD_EINT_INTERRUPT:
	    {
	        ret = gpio_eint_interrupt();
	        break;
	    }
	case CMD_EINT_GET_STATE:
	    {
	        eint_data.state = gpio_eint_state;
	        if(copy_to_user((void __user*)argp, (const void * )(&eint_data), sizeof(eint_data))) {
    			debug("copy_from_user failed !!!\n");
    			return -EFAULT;
    		}
            
	        break;
	    }
	default:
		debug("\nIllegal parameter cmd\n");
		return -EINVAL;
	}

	debug("digital_gpio_ioctl end\n");
	return ret;
}

static int digital_gpio_fasync (int fd, struct file *filp, int on)
{
    debug("digital_gpio_fasync\n");
	return fasync_helper(fd, filp, on, &gpio_async);
}

static irqreturn_t itead_gpio_int_handler(int irq, void *dev_id)
{
    //static int itead_gpio_int_cnt = 0;

    //printk("\nirq=%d kill_async times =%d\n", irq, ++itead_gpio_int_cnt);
    
    gpio_eint_state = PIO_INT_STA;
    
    if (gpio_eint_state) {

        gpio_eint_no_interrupt();

        kill_fasync(&gpio_async, SIGIO, POLL_IN);
        
        gpio_eint_interrupt();
        
        //printk("\ngpio_eint_state = 0x%0X\n", gpio_eint_state);
        
        return IRQ_HANDLED;
    } else {
        return IRQ_NONE;
    }
}

static int digital_gpio_open (struct inode * inode, struct file * file)
{	
	/* Only one task can get the lock of gpio_lock */
	if(!mutex_trylock(&gpio_lock)) {
		printk("\nfile:/dev/%s is busy now !\n",ITEAD_GPIO_INT_NODE);
		return -EBUSY;
	}
	
	gpio_eint_free_all();   
	
	debug("digital_gpio_open done\n");
	return 0;
}


static int digital_gpio_release (struct inode *node, struct file *file)
{
    
    digital_gpio_fasync(-1, file, 0);

    gpio_eint_free_all();
    
    /* Unlock gpio_lock after access */
	mutex_unlock(&gpio_lock);

	debug("\ndigital_gpio_release end\n");
	return 0;
}


static struct file_operations digital_gpio_fops = {
	.owner			= 	THIS_MODULE,
    .open       	=	digital_gpio_open,
    .unlocked_ioctl	=	digital_gpio_ioctl,
    .release		= 	digital_gpio_release,
    .fasync	        =   digital_gpio_fasync,
};

static int  itead_gpio_int_init(void)
{   
    int err;
	major = register_chrdev(0, "digital_gpio_device",&digital_gpio_fops);
	itead_gpio_int_class = class_create(THIS_MODULE, 
		(const char *)("itead_gpio_int_class"));
	
	itead_gpio_int_dev = device_create(itead_gpio_int_class, 
		(struct device *)NULL, 
		MKDEV(major, 0), 
		(void *)NULL, 
		(const char *)ITEAD_GPIO_INT_NODE);
	
	a10_a20_pio_base = (volatile uint32_t *)ioremap(A10_A20_PIO_BASE, 1024);

     err = request_irq(  GPIO_IRQ_NO, 
                        itead_gpio_int_handler,
				        IRQF_SHARED, 
				        "itead_gpio_int", 
				        &major);
	if (err) {
		pr_err("Can't request irq %d\n", GPIO_IRQ_NO);
	}


	debug("itead_gpio_int_init done\n");
	
	return 0;

}

static void itead_gpio_int_exit(void)
{
    
    free_irq(GPIO_IRQ_NO, &major);
    
	iounmap(a10_a20_pio_base);
	device_destroy(itead_gpio_int_class,MKDEV(major, 0));
	class_destroy(itead_gpio_int_class);
	unregister_chrdev(major,"digital_gpio_device");

	debug("itead_gpio_int_exit done\n");
}

module_init(itead_gpio_int_init);
module_exit(itead_gpio_int_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wu Pengfei");
MODULE_DESCRIPTION("itead_gpio_int driver");
MODULE_VERSION("V2.0");
