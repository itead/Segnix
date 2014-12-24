/**
 * @file itead_gpio_pin_map.h
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
#ifndef __ITEAD_GPIO_PIN_MAP_H__
#define __ITEAD_GPIO_PIN_MAP_H__

#include <assert.h>

#include <itead_config.h>
#include <itead_global.h>

#ifdef __cplusplus
extern "C"{
#endif


typedef struct pin_no_port_index_map{
	uint16_t port_no;
	uint16_t index;
} pin_no_port_index_map;


#if defined(BOARD_ITEADUINO_PLUS) || defined(BOARD_ITEAD_CORE_EVB)

#if defined(BOARD_ITEADUINO_PLUS)
#define PIN_MAX		138
#define PIN_MIN		9
#elif defined(BOARD_ITEAD_CORE_EVB)
#define PIN_MAX		138
#define PIN_MIN		0
#endif

#define GPIO_SIZE   (1*BLOCK_SIZE)
#define GPIO_BASE   (0x01c20800)

#define PORT_A	 	0
#define PORT_B		1
#define PORT_C		2
#define PORT_D		3
#define PORT_E		4
#define PORT_F	 	5
#define PORT_G		6
#define PORT_H		7
#define PORT_I		8

static const pin_no_port_index_map pnp[] = { 
    /*  port_no   index             pin_no      */   
#if defined(BOARD_ITEADUINO_PLUS)
	{PORT_NONE,	INDEX_NONE}	,	// 0
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_D,	0}	,	// 9
	{PORT_D,	1}	,	// 10
	{PORT_D,	2}	,	// 11
	{PORT_D,	3}	,	// 12
	{PORT_D,	4}	,	// 13
	{PORT_D,	5}	,	// 14
	{PORT_D,	6}	,	// 15
	{PORT_D,	7}	,	// 16
	{PORT_D,	8}	,	// 17
	{PORT_D,	9}	,	// 18
	{PORT_D,	10}	,	// 19
	{PORT_D,	11}	,	// 20
	{PORT_D,	12}	,	// 21
	{PORT_D,	13}	,	// 22
	{PORT_D,	14}	,	// 23
	{PORT_D,	15}	,	// 24
	{PORT_D,	16}	,	// 25
	{PORT_D,	17}	,	// 1		+ 25
	{PORT_D,	18}	,	// 2
	{PORT_D,	19}	,	// 3
	{PORT_D,	20}	,	// 4
	{PORT_D,	21}	,	// 5
	{PORT_D,	22}	,	// 6
	{PORT_D,	23}	,	// 7
	{PORT_D,	24}	,	// 8
	{PORT_D,	25}	,	// 9
	{PORT_D,	26}	,	// 10
	{PORT_D,	27}	,	// 11
	{PORT_H,	0}	,	// 12
	{PORT_H,	1}	,	// 13
	{PORT_H,	2}	,	// 14
	{PORT_H,	3}	,	// 15
	{PORT_H,	4}	,	// 16
	{PORT_H,	5}	,	// 17
	{PORT_H,	6}	,	// 18
	{PORT_H,	7}	,	// 19
	{PORT_H,	8}	,	// 20
	{PORT_H,	9}	,	// 21
	{PORT_H,	10}	,	// 22
	{PORT_H,	11}	,	// 23
	{PORT_H,	12}	,	// 24
	{PORT_H,	13}	,	// 25
	{PORT_H,	14}	,	// 1		+50
	{PORT_H,	15}	,	// 2
	{PORT_H,	16}	,	// 3
	{PORT_H,	17}	,	// 4
	{PORT_H,	18}	,	// 5
	{PORT_H,	19}	,	// 6
	{PORT_H,	20}	,	// 7
	{PORT_H,	21}	,	// 8
	{PORT_H,	22}	,	// 9
	{PORT_H,	23}	,	// 10
	{PORT_H,	24}	,	// 11
	{PORT_H,	25}	,	// 12
	{PORT_H,	26}	,	// 13
	{PORT_H,	27}	,	// 14
	{PORT_NONE,	INDEX_NONE}	,	// 15
	{PORT_NONE,	INDEX_NONE}	,	// 16
	{PORT_B,	18}	,	// 17
	{PORT_B,	19}	,	// 18
	{PORT_B,	20}	,	// 19
	{PORT_B,	21}	,	// 20
	{PORT_B,	22}	,	// 21
	{PORT_B,	23}	,	// 22
	{PORT_NONE,	INDEX_NONE}	,	// 23
	{PORT_NONE,	INDEX_NONE}	,	// 24
	{PORT_I,	19}	,	// 25
	{PORT_I,	18}	,	// 1		+75
	{PORT_I,	17}	,	// 2
	{PORT_I,	16}	,	// 3
	{PORT_G,	0}	,	// 4
	{PORT_G,	1}	,	// 5
	{PORT_G,	2}	,	// 6
	{PORT_G,	3}	,	// 7
	{PORT_G,	4}	,	// 8
	{PORT_G,	5}	,	// 9
	{PORT_G,	6}	,	// 10
	{PORT_G,	7}	,	// 11
	{PORT_G,	8}	,	// 12
	{PORT_G,	9}	,	// 13
	{PORT_G,	10}	,	// 14
	{PORT_G,	11}	,	// 15
	{PORT_E,	0}	,	// 16
	{PORT_E,	1}	,	// 17
	{PORT_E,	2}	,	// 18
	{PORT_E,	3}	,	// 19
	{PORT_E,	4}	,	// 20
	{PORT_E,	5}	,	// 21
	{PORT_E,	6}	,	// 22
	{PORT_E,	7}	,	// 23
	{PORT_E,	8}	,	// 24
	{PORT_E,	9}	,	// 25
	{PORT_E,	10}	,	// 1		+100
	{PORT_E,	11}	,	// 2
	{PORT_I,	0}	,	// 3
	{PORT_I,	1}	,	// 4
	{PORT_I,	2}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_I,	3}	,	// 7
	{PORT_B,	2}	,	// 8
	{PORT_I,	10}	,	// 9
	{PORT_I,	11}	,	// 10
	{PORT_I,	12}	,	// 11
	{PORT_I,	13}	,	// 12
	{PORT_I,	4}	,	// 13
	{PORT_I,	5}	,	// 14
	{PORT_I,	6}	,	// 15
	{PORT_I,	7}	,	// 16
	{PORT_I,	8}	,	// 17
	{PORT_I,	9}	,	// 18
	{PORT_I,	20}	,	// 19
	{PORT_I,	21}	,	// 20
	{PORT_B,	3}	,	// 21
	{PORT_B,	4}	,	// 22
	{PORT_B,	5}	,	// 23
	{PORT_B,	6}	,	// 24
	{PORT_B,	7}	,	// 25
	{PORT_B,	8}	,	// 1		+125
	{PORT_B,	9}	,	// 2
	{PORT_B,	10}	,	// 3
	{PORT_B,	11}	,	// 4
	{PORT_B,	12}	,	// 5
	{PORT_B,	13}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_B,	14}	,	// 8
	{PORT_B,	15}	,	// 9
	{PORT_B,	16}	,	// 10
	{PORT_B,	17}	,	// 11
	{PORT_I,	14}	,	// 12
	{PORT_I,	15}	,	// 13
#elif defined(BOARD_ITEAD_CORE_EVB)
    {PORT_B, 5	}, /* 0  */
    {PORT_B, 6	}, /* 1  */
    {PORT_B, 7	}, /* 2  */
    {PORT_B, 8	}, /* 3  */
    {PORT_B, 18	}, /* 4  */
    {PORT_B, 19	}, /* 5  */
    {PORT_B, 20	}, /* 6  */
    {PORT_B, 21	}, /* 7  */
    {PORT_B, 22	}, /* 8  */
    {PORT_B, 23	}, /* 9  */
    {PORT_D, 0	}, /* 10 */
    {PORT_D, 1	}, /* 11 */
    {PORT_D, 2	}, /* 12 */
    {PORT_D, 3	}, /* 13 */
    {PORT_D, 4	}, /* 14 */
    {PORT_D, 5	}, /* 15 */
    {PORT_D, 6	}, /* 16 */
    {PORT_D, 7	}, /* 17 */
    {PORT_D, 8	}, /* 18 */
    {PORT_D, 9	}, /* 19 */
    {PORT_D, 10	}, /* 20 */
    {PORT_D, 11	}, /* 21 */
    {PORT_D, 12	}, /* 22 */
    {PORT_D, 13	}, /* 23 */
    {PORT_D, 14	}, /* 24 */
    {PORT_D, 15	}, /* 25 */
    {PORT_D, 16	}, /* 26 */
    {PORT_D, 17	}, /* 27 */
    {PORT_D, 18	}, /* 28 */
    {PORT_D, 19	}, /* 29 */
    {PORT_D, 20	}, /* 30 */
    {PORT_D, 21	}, /* 31 */
    {PORT_D, 22	}, /* 32 */
    {PORT_D, 23	}, /* 33 */
    {PORT_D, 24	}, /* 34 */
    {PORT_D, 25	}, /* 35 */
    {PORT_D, 26	}, /* 36 */
    {PORT_D, 27	}, /* 37 */
    {PORT_G, 0	}, /* 38 */
    {PORT_G, 1	}, /* 39 */
    {PORT_G, 2	}, /* 40 */
    {PORT_G, 3	}, /* 41 */
    {PORT_G, 4	}, /* 42 */
    {PORT_G, 5	}, /* 43 */
    {PORT_G, 6	}, /* 44 */
    {PORT_G, 7	}, /* 45 */
    {PORT_G, 8	}, /* 46 */
    {PORT_G, 9	}, /* 47 */
    {PORT_G, 10	}, /* 48 */
    {PORT_G, 11	}, /* 49 */
    {PORT_NONE, INDEX_NONE }, /* 50 */
    {PORT_NONE, INDEX_NONE	}, /* 51 */
    {PORT_NONE, INDEX_NONE	}, /* 52 */
    {PORT_NONE, INDEX_NONE	}, /* 53 */
    {PORT_NONE, INDEX_NONE	}, /* 54 */
    {PORT_NONE, INDEX_NONE	}, /* 55 */
    {PORT_NONE, INDEX_NONE	}, /* 56 */
    {PORT_NONE, INDEX_NONE	}, /* 57 */
    {PORT_NONE, INDEX_NONE	}, /* 58 */
    {PORT_NONE, INDEX_NONE	}, /* 59 */
    {PORT_I, 20	}, /* 60 */
    {PORT_I, 21	}, /* 61 */
    {PORT_B, 2  }, /* 62 */
    {PORT_H, 27 }, /* 63 */

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

#endif
};

#elif defined(BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)

#if defined(BOARD_RASPBERRY_PI_RV2)
#define PIN_MAX		32
#elif defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
#define PIN_MAX		35
#endif

#define PIN_MIN		3
#define GPIO_SIZE   (1*BLOCK_SIZE)
#define BCM2708_PERI_BASE           (0x20000000)
#define GPIO_BASE		            (BCM2708_PERI_BASE + 0x00200000)
#define GPIO_GPFSEL_OFFSET          (0x00)
#define GPIO_GPSET_OFFSET           (0x1c)
#define GPIO_GPCLR_OFFSET           (0x28)
#define GPIO_GPLEV_OFFSET           (0x34)

#define PORT_GPIO   0

static const pin_no_port_index_map pnp[] = {
/*	port_no	  index				pin_no 		*/    
	{PORT_NONE,	INDEX_NONE}	,	// 0
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_GPIO,	2}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_GPIO,	3}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_GPIO,	4}	,	// 7
	{PORT_GPIO,	14}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
	{PORT_GPIO,	15}	,	// 10
	{PORT_GPIO,	17}	,	// 1     + 10
	{PORT_GPIO,	18}	,	// 2
	{PORT_GPIO,	27}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_GPIO,	22}	,	// 5
	{PORT_GPIO,	23}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_GPIO,	24}	,	// 8
	{PORT_GPIO,	10}	,	// 9
	{PORT_NONE,	INDEX_NONE}	,	// 10
	{PORT_GPIO,	9}	,	// 1     + 20
	{PORT_GPIO,	25}	,	// 2
	{PORT_GPIO,	11}	,	// 3
	{PORT_GPIO,	8}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_GPIO,	7}	,	// 6
#if defined(BOARD_RASPBERRY_PI_RV2)	
	{PORT_NONE,	INDEX_NONE}	,	// 27
	{PORT_NONE,	INDEX_NONE}	,	// 28
	{PORT_GPIO,	28}	,	// 29
	{PORT_GPIO,	29}	,	// 30  
	{PORT_GPIO,	30}	,	// 31
	{PORT_GPIO,	31}	,	// 32
#elif defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    {PORT_GPIO,	5 }	,	// 27
    {PORT_GPIO,	6 }	,	// 28
    {PORT_GPIO,	12}	,	// 29
    {PORT_GPIO,	13}	,	// 30
    {PORT_GPIO,	16}	,	// 31
    {PORT_GPIO,	19}	,	// 32
    {PORT_GPIO,	20}	,	// 33
    {PORT_GPIO,	21}	,	// 34
    {PORT_GPIO,	26}	,	// 35
#endif
};

#elif defined(BOARD_BEAGLEBONEBLACK)
    
#define PIN_MAX		117
#define PIN_MIN		7

#define GPIO0_BASE  (0x44E07000)
#define GPIO1_BASE  (0x4804C000)
#define GPIO1_END   (0x4804DFFF)  
#define GPIO2_BASE  (0x481AC000)
#define GPIO3_BASE  (0x441AE000)
#define GPIO_SIZE   (GPIO1_END - GPIO1_BASE)
  
#define GPIO_OE           (0x134)
#define GPIO_DATAIN       (0x138)
#define GPIO_DATAOUT      (0x13C)
#define GPIO_CLEARDATAOUT (0x190)
#define GPIO_SETDATAOUT   (0x194)

#define PORT_GPIO0   0
#define PORT_GPIO1   1
#define PORT_GPIO2   2
#define PORT_GPIO3   3

static const pin_no_port_index_map pnp[] = {
/*	port_no	  index				pin_no 		*/    
	{PORT_NONE,	INDEX_NONE}	,	// 0
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_GPIO0 ,        7} ,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
    {PORT_NONE,	INDEX_NONE}	,	// 0 + 10
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
  	{PORT_NONE,	INDEX_NONE}	,	// 0 + 20
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_GPIO0,	    22}	,	// 2
	{PORT_GPIO0,	    23}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_GPIO0,	    26}	,	// 6
	{PORT_GPIO0,	    27}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 30
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 40
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_GPIO1,	    12}	,	// 4
	{PORT_GPIO1,	    13}	,	// 5
	{PORT_GPIO1,	    14}	,	// 6
	{PORT_GPIO1,	    15}	,	// 7
	{PORT_GPIO1,	    16}	,	// 8
	{PORT_GPIO1,	    17}	,	// 9
   	{PORT_GPIO1,	    18}	,	// 0 + 50
	{PORT_GPIO1,	    19}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_GPIO1,	    28}	,	// 0 + 60
	{PORT_GPIO1,	    29}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_GPIO2,	     1}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 70
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 80
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 90
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 100
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 110
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_GPIO3,	    19}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_GPIO3,	    21}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 120
	{PORT_NONE,	INDEX_NONE}	,	// 1
	{PORT_NONE,	INDEX_NONE}	,	// 2
	{PORT_NONE,	INDEX_NONE}	,	// 3
	{PORT_NONE,	INDEX_NONE}	,	// 4
	{PORT_NONE,	INDEX_NONE}	,	// 5
	{PORT_NONE,	INDEX_NONE}	,	// 6
	{PORT_NONE,	INDEX_NONE}	,	// 7
	{PORT_NONE,	INDEX_NONE}	,	// 8
	{PORT_NONE,	INDEX_NONE}	,	// 9
   	{PORT_NONE,	INDEX_NONE}	,	// 0 + 130
	{PORT_NONE,	INDEX_NONE}		// 1
};
#else
    assert(! "Unsupported Board. You need to choose a valid one.");
#endif


#ifdef __cplusplus
}
#endif

#endif
