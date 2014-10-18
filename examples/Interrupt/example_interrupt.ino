/**
 * @example example_interrupt.ino
 *
 * Show usage of attachInterrupt/detachInterrupt/noInterrupt/interrupt functions.
 * 
 * @warning 
 *  ONLY works on ITEAD-OS for now !
 *  You need to load itead_gpio_int.ko to enable interrupt functions, such as
 *  attachInterrupt, detachInterrupt, noInterrupt, interrupt, etc.
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

static void gpioShareIsr(uint16_t pin, void *pdata)
{
    printf("\n============ gpio_share_isr begin===========\n");
    printf("pin = %d\n", pin);
    if( pdata == NULL) {
        printf("pdata == NULL\n");
    } else {
        printf("*pdata = %s\n", (char *)pdata);
    }
    printf("============ gpio_share_isr   end===========\n");
}

void setup(void)
{
    static unsigned long main_loop_counter = 0;
    char *msg1 = (char *)"This is pin1";
    char *msg2 = (char *)"This is pin2";
    uint16_t pin1 = 37;
    uint16_t pin2 = 39;
    
    attachInterrupt(pin1, gpioShareIsr, (void *)msg1, GPIO_EINT_CFG_FALLING);
    attachInterrupt(pin2, gpioShareIsr, (void *)msg2, GPIO_EINT_CFG_RAISING);

    while (main_loop_counter < 10)
    {
        main_loop_counter++;
        if (main_loop_counter == 4 && (noInterrupt()== 0)) {
            printf("\n=====No Interrupt======\n");
        }

        if (main_loop_counter == 6 && (interrupt()== 0)) {
            printf("\n=====Interrupt======\n");
        }
        printf("main_loop_counter = %lu\n", main_loop_counter);
        sleep(5);
    }
    detachInterrupt(pin1);
    detachInterrupt(pin2);
    
}

void loop(void)
{
    exit(0);
}
