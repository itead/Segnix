/**
 * @file itead_interrupt.h
 *
 * Porvide implementation of Arduino Interrupt API
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/8/14
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include <itead_config.h>

#if defined(BOARD_ITEADUINO_PLUS)

#include <itead_interrupt.h>
#include <itead_global.h>

#define GPIO_EINT_NUMBER_TOTAL      (32)

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
    { 37, 0 },
    { 39, 2 },
    { 40, 3 },
    { 41, 4 },
    { 42, 5 },
    { 43, 6 },
    { 44, 7 },
    { 45, 8 },
    { 46, 9 },
    { 47, 10 },
    { 48, 11 },
    { 49, 12 },
    { 50, 13 },
    { 51, 14 },
    { 52, 15 },
    { 53, 16 },
    { 54, 17 },
    { 55, 18 },
    { 56, 19 },
    { 57, 20 },
    { 58, 21 },
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


#define ITEAD_GPIO_INT_NODE_PATH     ("/dev/itead_gpio_int")

typedef struct gpio_eint_ioctl_t {
    uint16_t pin;
    GpioEintMode mode;
    uint32_t state;
} gpio_eint_ioctl_t;

typedef struct GpioEint {
    uint16_t pin;           /**< The pin number of Iteaduino Plus A20 */
    gpio_eint_isr_t isr;    /**< Interrupt Service Routine */
    void *pdata;            /**< Parameter passed to isr when eint occurs */
} GpioEint;


static GpioEint gpio_eint[GPIO_EINT_NUMBER_TOTAL];
static int      gpio_node = -1;

static void signalSigioServer(int signum)
{
    static int i;
    //static int cnt = 0;
    static gpio_eint_ioctl_t eint_data;
    
    noInterrupt();
    
    //cnt++;
    //printf("deal with SIGIO cnt=%d\n", cnt);
    
    if (ioctl(gpio_node, CMD_EINT_GET_STATE, &eint_data) < 0) {
        sdkerr("ioctl(gpio_node, CMD_EINT_GET_STATE, &eint_data) failed !\n");
        goto signalSigioServer_out;
    }
    //printf("CMD_EINT_GET_STATE return 0x%X\n", eint_data.state);

    if (!eint_data.state) {
        goto signalSigioServer_out;
    }
    
    /* EINT0 has the highest priority */
    for (i=0; i<GPIO_EINT_NUMBER_TOTAL; i++) {
        if (eint_data.state & (0x1<<i)) {
            if (gpio_eint[i].isr) {
                (*gpio_eint[i].isr)(gpio_eint[i].pin, gpio_eint[i].pdata);
            }
        }
    }

signalSigioServer_out:
    interrupt();
    return;
}

static inline registerSigio(void)
{
    int oflags;
    gpio_node = open(ITEAD_GPIO_INT_NODE_PATH, O_RDWR);
	if (gpio_node < 0)
	{
		sdkerr("can't open file: %s !\n", ITEAD_GPIO_INT_NODE_PATH);
		abort();
	}
	
    if (signal(SIGIO, signalSigioServer) == SIG_ERR) {
        sdkerr("signal(SIGIO, signalSigioServer) failed !\n");
		abort();
    }
	fcntl(gpio_node, F_SETOWN, getpid());
	oflags = fcntl(gpio_node, F_GETFL); 
	fcntl(gpio_node, F_SETFL, oflags | FASYNC);
	
}

/**
 * Specifies a named Interrupt Service Routine (ISR) to call when an interrupt  occurs.
 * Replaces any previous function that was attached to the interrupt.
 * 
 * @param pin - the pin number
 * @param isr - the ISR to call when interrupt occurs.
 * @param mode - RAISING/FALLING/HIGH/LOW/DEDGE
 * 
 * @retval 0 - success
 * @retval negative - failed
 */
int32_t attachInterrupt(uint16_t pin, gpio_eint_isr_t isr, void *pdata, uint32_t mode)
{
    gpio_eint_ioctl_t eint_data;
    uint32_t eint_no;

    eint_no = get_eint_no(pin);
    if (eint_no == GPIO_EINT_NONE) {
        sdkerr("Illegal parameter pin=%d\n", pin);
        return -1;
    }
    
    gpio_eint[eint_no].pin = pin;
    gpio_eint[eint_no].isr = isr;
    gpio_eint[eint_no].pdata = pdata;
    
    eint_data.pin = pin;
    eint_data.mode = mode;

    if(gpio_node < 0) {
        registerSigio();
    }
    if (ioctl(gpio_node, CMD_EINT_REQUEST, &eint_data) < 0) {
        sdkerr("ioctl(gpio_node, CMD_EINT_REQUEST, &eint_data) failed !\n");
        return -1;
    }

    return 0;
}

/**
 * @param pin - the pin number
 * 
 * @retval 0 - success
 * @retval negative - failed
 */
int32_t detachInterrupt(uint16_t pin)
{
    gpio_eint_ioctl_t eint_data;
    uint32_t eint_no;
    
    eint_no = get_eint_no(pin);
    if (eint_no == GPIO_EINT_NONE) {
        sdkerr("Illegal parameter pin=%d\n", pin);
        return -1;
    }
    
    gpio_eint[eint_no].pin = GPIO_EINT_NONE;
    gpio_eint[eint_no].isr = NULL;
    gpio_eint[eint_no].pdata = NULL;
    
    eint_data.pin = pin;
    
    if (gpio_node < 0) {
        registerSigio();
    }
    if (ioctl(gpio_node, CMD_EINT_FREE, &eint_data) < 0) {
        sdkerr("ioctl(gpio_node, CMD_EINT_FREE, &eint_data) failed !\n");
        return -1;
    }
    
    return 0;
}

/**
 * @retval 0 - success
 * @retval negative - failed
 */
int32_t noInterrupt(void)
{
    if(gpio_node < 0) {
        registerSigio();
    }
    return ioctl(gpio_node, CMD_EINT_NO_INTERRUPT);
}


/**
 * @retval 0 - success
 * @retval negative - failed
 */
int32_t interrupt(void)
{
    if(gpio_node < 0) {
        registerSigio();
    }
    return ioctl(gpio_node, CMD_EINT_INTERRUPT);
}

#endif /* #if defined(BOARD_ITEADUINO_PLUS) */
