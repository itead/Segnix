/**
 * @file itead_gpio.h
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
#ifndef __ITEAD_GPIO_H__
#define __ITEAD_GPIO_H__

#include <stdint.h>
#include <itead_config.h>

#ifdef __cplusplus
extern "C"{
#endif

/* 
 * You can manipulate a group of gpio pins as STRUCT_8BITS_BUS.
 * Dn(n=0-7) is the corresponding bit of bus. Actually, every bit
 * of bus is related to a unique pin. Make sure that all the pins 
 * you are using is can be set to INPUT mode or OUTPUT mode.
 */
typedef struct
{
	uint32_t D0;
	uint32_t D1;
	uint32_t D2;
	uint32_t D3;
	uint32_t D4;
	uint32_t D5;
	uint32_t D6;
	uint32_t D7;
}STRUCT_8BITS_BUS;

/* 
 * You can manipulate a group of gpio pins as STRUCT_16BITS_BUS.
 * Dn(n=0-15) is the corresponding bit of bus. Actually, every bit
 * of bus is related to a unique pin. Make sure that all the pins 
 * you are using is can be set to INPUT mode or OUTPUT mode.
 */
typedef struct
{
	uint32_t D0;
	uint32_t D1;
	uint32_t D2;
	uint32_t D3;
	uint32_t D4;
	uint32_t D5;
	uint32_t D6;
	uint32_t D7;
	uint32_t D8;
	uint32_t D9;
	uint32_t D10;
	uint32_t D11;
	uint32_t D12;
	uint32_t D13;
	uint32_t D14;
	uint32_t D15;
}STRUCT_16BITS_BUS;


/*
 * You should always replace 1 with HIGH.
 * DO NOT modify the defines of HIGH,LOW,INPUT,OUTPUT.
 */

/**
 * @addtogroup gpio
 * @{
 */
#define HIGH		(0x1)   /**< HIGH = 0x1  */
#define LOW		(0x0)   /**< LOW = 0x0  */
#define INPUT 		(0x0)   /**< INPUT = 0x0  */
#define OUTPUT 		(0x1)   /**< OUTPUT = 0x1  */
/** @} */


/**
 * @ingroup gpio
 * @defgroup gpioPinName Available Pin Name
 * @details Accroding to different boards, user can use pin name below directly, intead of a number, 
 *  since these have been defined in Segnix. When calling pinMode/digitalRead/digitalWrite/analogWrite 
 *  functions, user can pass pin name replacing a number.
 * 
 * @par On Iteaduino Plus
 * -# PD0	
 * -# PD1	
 * -# PD2	
 * -# PD3	
 * -# PD4	
 * -# PD5	
 * -# PD6	
 * -# PD7	
 * -# PD8	
 * -# PD9	
 * -# PD10	
 * -# PD11	
 * -# PD12	
 * -# PD13	
 * -# PD14	
 * -# PD15	
 * -# PD16	
 * -# PD17	
 * -# PD18	
 * -# PD19	
 * -# PD20	
 * -# PD21	
 * -# PD22	
 * -# PD23	
 * -# PD24	
 * -# PD25	
 * -# PD26	
 * -# PD27	
 * -# PH0	
 * -# PH1	
 * -# PH2	
 * -# PH3	
 * -# PH4	
 * -# PH5	
 * -# PH6	
 * -# PH7	
 * -# PH8	
 * -# PH9	
 * -# PH10	
 * -# PH11	
 * -# PH12	
 * -# PH13	
 * -# PH14	
 * -# PH15	
 * -# PH16	
 * -# PH17	
 * -# PH18	
 * -# PH19	
 * -# PH20	
 * -# PH21	
 * -# PH22	
 * -# PH23	
 * -# PH24	
 * -# PH25	
 * -# PH26	
 * -# PH27	
 * -# PB18	
 * -# PB19	
 * -# PB20	
 * -# PB21	
 * -# PB22	
 * -# PB23	
 * -# PI19	
 * -# PI18	
 * -# PI17	
 * -# PI16	
 * -# PG0	
 * -# PG1	
 * -# PG2	
 * -# PG3	
 * -# PG4	
 * -# PG5	
 * -# PG6	
 * -# PG7	
 * -# PG8	
 * -# PG9	
 * -# PG10	
 * -# PG11	
 * -# PE0	
 * -# PE1	
 * -# PE2	
 * -# PE3	
 * -# PE4	
 * -# PE5	
 * -# PE6	
 * -# PE7	
 * -# PE8	
 * -# PE9	
 * -# PE10	
 * -# PE11	
 * -# PI0	
 * -# PI1	
 * -# PI2	
 * -# PI3	
 * -# PB2	
 * -# PI10	
 * -# PI11	
 * -# PI12	
 * -# PI13	
 * -# PI4	
 * -# PI5	
 * -# PI6	
 * -# PI7	
 * -# PI8	
 * -# PI9	
 * -# PI20	
 * -# PI21	
 * -# PB3	
 * -# PB4	
 * -# PB5	
 * -# PB6	
 * -# PB7	
 * -# PB8	
 * -# PB9	
 * -# PB10	
 * -# PB11	
 * -# PB12	
 * -# PB13	
 * -# PB14	
 * -# PB15	
 * -# PB16	
 * -# PB17	
 * -# PI14	
 * -# PI15	
 *
 * @par On ITEAD CORE EVB
 * -# PB2
 * -# PB5
 * -# PB6	 
 * -# PB7	 
 * -# PB8	 
 * -# PB18	
 * -# PB19	
 * -# PB20	
 * -# PB21	
 * -# PB22	
 * -# PB23	
 * -# PD0	 
 * -# PD1	 
 * -# PD2	 
 * -# PD3	 
 * -# PD4	 
 * -# PD5	 
 * -# PD6	 
 * -# PD7	 
 * -# PD8	 
 * -# PD9	 
 * -# PD10	
 * -# PD11	
 * -# PD12	
 * -# PD13	
 * -# PD14	
 * -# PD15	
 * -# PD16	
 * -# PD17	
 * -# PD18	
 * -# PD19	
 * -# PD20	
 * -# PD21	
 * -# PD22	
 * -# PD23	
 * -# PD24	
 * -# PD25	
 * -# PD26	
 * -# PD27	
 * -# PG0	 
 * -# PG1	 
 * -# PG2	 
 * -# PG3	 
 * -# PG4	 
 * -# PG5	 
 * -# PG6	 
 * -# PG7	 
 * -# PG8	 
 * -# PG9	 
 * -# PG10	
 * -# PG11	
 * -# PI10	
 * -# PI11	
 * -# PI12	
 * -# PI13	
 * -# PI14	
 * -# PI15	
 * -# PI16	
 * -# PI17	
 * -# PI18	
 * -# PI19	
 * -# PI20	
 * -# PI21	
 * -# PH27
 *
 * @par On Raspberry Rv2
 * -# GPIO2 
 * -# GPIO3 
 * -# GPIO4 
 * -# GPIO14
 * -# GPIO15
 * -# GPIO17
 * -# GPIO18
 * -# GPIO27
 * -# GPIO22
 * -# GPIO23
 * -# GPIO24
 * -# GPIO10
 * -# GPIO9 
 * -# GPIO25
 * -# GPIO11
 * -# GPIO8 
 * -# GPIO7 
 * -# GPIO28
 * -# GPIO29
 * -# GPIO30
 * -# GPIO31
 *
 * @par On Raspberry Pi Model B+
 * -# GPIO2 
 * -# GPIO3 
 * -# GPIO4 
 * -# GPIO14
 * -# GPIO15
 * -# GPIO17
 * -# GPIO18
 * -# GPIO27
 * -# GPIO22
 * -# GPIO23
 * -# GPIO24
 * -# GPIO10
 * -# GPIO9 
 * -# GPIO25
 * -# GPIO11
 * -# GPIO8 
 * -# GPIO7 
 * -# GPIO5 
 * -# GPIO6 
 * -# GPIO12
 * -# GPIO13
 * -# GPIO16
 * -# GPIO19
 * -# GPIO20
 * -# GPIO21
 * -# GPIO26
 *
 * @par On BeagleBoneBlack
 * -# GPIO0_22
 * -# GPIO0_23 
 * -# GPIO0_26 
 * -# GPIO0_27 
 * -# GPIO1_12
 * -# GPIO1_13
 * -# GPIO1_14
 * -# GPIO1_15
 * -# GPIO1_16
 * -# GPIO1_17
 * -# GPIO1_18
 * -# GPIO1_19
 * -# GPIO1_28
 * -# GPIO1_29
 * -# GPIO2_1
 * -# GPIO3_19 
 * -# GPIO3_21
 *
 */

#if defined(BOARD_ITEADUINO_PLUS)

#define PD0		9
#define PD1		10
#define PD2		11
#define PD3		12
#define PD4		13
#define PD5		14
#define PD6		15
#define PD7		16
#define PD8		17
#define PD9		18
#define PD10	19	
#define PD11	20	
#define PD12	21	
#define PD13	22	
#define PD14	23	
#define PD15	24	
#define PD16	25	
#define PD17	26	
#define PD18	27	
#define PD19	28	
#define PD20	29	
#define PD21	30	
#define PD22	31	
#define PD23	32	
#define PD24	33	
#define PD25	34	
#define PD26	35
#define PD27	36
#define PH0		37
#define PH1		38
#define PH2		39
#define PH3		40
#define PH4		41
#define PH5		42
#define PH6		43
#define PH7		44
#define PH8		45
#define PH9		46
#define PH10	47
#define PH11	48
#define PH12	49
#define PH13	50
#define PH14	51
#define PH15	52
#define PH16	53
#define PH17	54
#define PH18	55
#define PH19	56
#define PH20	57
#define PH21	58
#define PH22	59
#define PH23	60
#define PH24	61
#define PH25	62
#define PH26	63
#define PH27	64

#define PB18	67
#define PB19	68
#define PB20	69
#define PB21	70
#define PB22	71
#define PB23	72

#define PI19	75	
#define PI18	76
#define PI17	77
#define PI16	78
#define PG0		79
#define PG1		80
#define PG2		81
#define PG3		82
#define PG4		83
#define PG5		84
#define PG6		85
#define PG7		86
#define PG8		87
#define PG9		88
#define PG10	89
#define PG11	90
#define PE0		91
#define PE1		92
#define PE2		93
#define PE3		94
#define PE4		95
#define PE5		96
#define PE6		97
#define PE7		98
#define PE8		99
#define PE9		100
#define PE10	101
#define PE11	102
#define PI0		103
#define PI1		104
#define PI2		105

#define PI3		107
#define PB2		108	
#define PI10	109
#define PI11	110
#define PI12	111
#define PI13	112
#define PI4		113
#define PI5		114
#define PI6		113
#define PI7		116
#define PI8		117
#define PI9		118
#define PI20	119
#define PI21	120
#define PB3		121
#define PB4		122
#define PB5		123
#define PB6		124
#define PB7		125
#define PB8		126
#define PB9		127
#define PB10	128
#define PB11	129
#define PB12	130
#define PB13	131

#define PB14	133
#define PB15	134
#define PB16	135
#define PB17	136
#define PI14	137
#define PI15	138

#elif defined (BOARD_ITEAD_CORE_EVB)

#define PB5	    (0)
#define PB6	    (1)
#define PB7	    (2)
#define PB8	    (3)
#define PB18	(4)
#define PB19	(5)
#define PB20	(6)
#define PB21	(7)
#define PB22	(8)
#define PB23	(9)
#define PD0	    (10)
#define PD1	    (11)
#define PD2	    (12)
#define PD3	    (13)
#define PD4	    (14)
#define PD5	    (15)
#define PD6	    (16)
#define PD7	    (17)
#define PD8	    (18)
#define PD9	    (19)
#define PD10	(20)
#define PD11	(21)
#define PD12	(22)
#define PD13	(23)
#define PD14	(24)
#define PD15	(25)
#define PD16	(26)
#define PD17	(27)
#define PD18	(28)
#define PD19	(29)
#define PD20	(30)
#define PD21	(31)
#define PD22	(32)
#define PD23	(33)
#define PD24	(34)
#define PD25	(35)
#define PD26	(36)
#define PD27	(37)
#define PG0	    (38)
#define PG1	    (39)
#define PG2	    (40)
#define PG3	    (41)
#define PG4	    (42)
#define PG5	    (43)
#define PG6	    (44)
#define PG7	    (45)
#define PG8	    (46)
#define PG9	    (47)
#define PG10	(48)
#define PG11	(49)
#define PI10	(109)   /* itead_gpio_int.c 22 */
#define PI11	(110)   /* itead_gpio_int.c 23 */
#define PI12	(111)   /* itead_gpio_int.c 24 */
#define PI13	(112)   /* itead_gpio_int.c 25 */
#define PI14	(137)   /* itead_gpio_int.c 26 */
#define PI15	(138)   /* itead_gpio_int.c 27 */
#define PI16	(78)   /* itead_gpio_int.c 28 */
#define PI17	(77)   /* itead_gpio_int.c 29 */
#define PI18	(76)   /* itead_gpio_int.c 30 */
#define PI19	(75)   /* itead_gpio_int.c 31 */
#define PI20	(60)
#define PI21	(61)
#define PB2     (62)
#define PH27    (63)

#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)

#define GPIO2       3 
#define GPIO3       5 
#define GPIO4       7 
#define GPIO14      8 
#define GPIO15      10
#define GPIO17      11
#define GPIO18      12
#define GPIO27      13
#define GPIO22      15
#define GPIO23      16
#define GPIO24      18
#define GPIO10      19
#define GPIO9       21
#define GPIO25      22
#define GPIO11      23
#define GPIO8       24
#define GPIO7       26
#if defined(BOARD_RASPBERRY_PI_RV2)
#define GPIO28      29
#define GPIO29      30
#define GPIO30      31
#define GPIO31      32
#elif defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
#define GPIO5       (27)
#define GPIO6       (28)
#define GPIO12      (29)
#define GPIO13      (30)
#define GPIO16      (31)
#define GPIO19      (32)
#define GPIO20      (33)
#define GPIO21      (34)
#define GPIO26      (35)
#endif /* defined(BOARD_RASPBERRY_PI_RV2) */

#elif defined (BOARD_BEAGLEBONEBLACK)

#define GPIO_NUM(port,num)  (port)*32+(num)

#define GPIO0_7   GPIO_NUM(0,7)
#define GPIO0_22  GPIO_NUM(0,22)
#define GPIO0_23  GPIO_NUM(0,23)
#define GPIO0_26  GPIO_NUM(0,26)
#define GPIO0_27  GPIO_NUM(0,27)
#define GPIO1_12  GPIO_NUM(1,12)
#define GPIO1_13  GPIO_NUM(1,13)
#define GPIO1_14  GPIO_NUM(1,14)
#define GPIO1_15  GPIO_NUM(1,15)
#define GPIO1_16  GPIO_NUM(1,16)
#define GPIO1_17  GPIO_NUM(1,17)
#define GPIO1_18  GPIO_NUM(1,18)
#define GPIO1_19  GPIO_NUM(1,19)
#define GPIO1_28  GPIO_NUM(1,28)
#define GPIO1_29  GPIO_NUM(1,29)
#define GPIO2_1   GPIO_NUM(2,1)
#define GPIO3_19  GPIO_NUM(3,19)
#define GPIO3_21  GPIO_NUM(3,21)


#endif 

/**
 * @ingroup gpio
 * @defgroup adcChannelName Available ADC pin
 * @details Accroding to different boards, user can use adc pin below directly, intead of a number, 
 *  since these have been defined in Segnix. When calling analogRead() functions, user can pass ADC  
 *  pin replacing a number.
 * 
 * @par On BeagleBoneBlack
 * -# AIN0
 * -# AIN1
 * -# AIN2
 * -# AIN3
 * -# AIN4
 * -# AIN5
 * -# AIN6
 * -# AIN7
 *
 */


#if defined (BOARD_BEAGLEBONEBLACK)    
/* Only define number of channels */
#define AIN0        0
#define AIN1        1
#define AIN2        2
#define AIN3        3
#define AIN4        4
#define AIN5        5
#define AIN6        6
#define AIN7        7
#endif  

/**
 * @addtogroup gpio
 * @{
 */

uint32_t pinMode(uint16_t pin, uint8_t mode);
uint32_t digitalWrite(uint16_t pin, uint8_t val);
uint32_t digitalRead(uint16_t pin);
uint32_t analogWrite(uint16_t pin, uint8_t duty);
uint32_t analogRead(uint32_t pin);
uint32_t set8BitsBUS(STRUCT_8BITS_BUS *bus,
	uint16_t DB0, uint16_t DB1, uint16_t DB2, uint16_t DB3,
	uint16_t DB4, uint16_t DB5, uint16_t DB6, uint16_t DB7,
	uint8_t mode);

uint32_t set16BitsBUS(STRUCT_16BITS_BUS *bus, 
	uint16_t DB0, uint16_t DB1, uint16_t DB2, uint16_t DB3,
	uint16_t DB4, uint16_t DB5, uint16_t DB6, uint16_t DB7,
	uint16_t DB8, uint16_t DB9, uint16_t DB10,uint16_t DB11,
	uint16_t DB12,uint16_t DB13,uint16_t DB14,uint16_t DB15,
	uint8_t mode);

uint32_t digitalWrite8(STRUCT_8BITS_BUS *bus, uint8_t val);
uint32_t digitalWrite16(STRUCT_16BITS_BUS *bus, uint16_t val);
uint8_t digitalRead8(STRUCT_8BITS_BUS *bus);
uint16_t digitalRead16(STRUCT_16BITS_BUS *bus);

/** @} */

    


#ifdef __cplusplus
} // extern "C"
#endif


#endif
