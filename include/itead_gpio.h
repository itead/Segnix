/********************************************************************
* File		:	itead_gpio.h
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
 * Wu Pengfei<pengfei.wu@itead.cc>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
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
#define HIGH		0x1
#define LOW			0x0

#define INPUT 		0x0
#define OUTPUT 		0x1


#ifdef BOARD_ITEADUINO_PLUS

#define SYSLED1		58
#define SYSLED2		57

/*
 * You should always replace 9 with PIN_PD0.
 * DO NOT modify the defines of all the pin numbers.
 * In your program, it's a bad idea to pass a pin number
 * without defines here to functions related to.
 */
#define PIN_PD0		9
#define PIN_PD1		10
#define PIN_PD2		11
#define PIN_PD3		12
#define PIN_PD4		13
#define PIN_PD5		14
#define PIN_PD6		15
#define PIN_PD7		16
#define PIN_PD8		17
#define PIN_PD9		18
#define PIN_PD10	19	
#define PIN_PD11	20	
#define PIN_PD12	21	
#define PIN_PD13	22	
#define PIN_PD14	23	
#define PIN_PD15	24	
#define PIN_PD16	25	
#define PIN_PD17	26	
#define PIN_PD18	27	
#define PIN_PD19	28	
#define PIN_PD20	29	
#define PIN_PD21	30	
#define PIN_PD22	31	
#define PIN_PD23	32	
#define PIN_PD24	33	
#define PIN_PD25	34	
#define PIN_PD26	35
#define PIN_PD27	36
#define PIN_PH0		37
#define PIN_PH1		38
#define PIN_PH2		39
#define PIN_PH3		40
#define PIN_PH4		41
#define PIN_PH5		42
#define PIN_PH6		43
#define PIN_PH7		44
#define PIN_PH8		45
#define PIN_PH9		46
#define PIN_PH10	47
#define PIN_PH11	48
#define PIN_PH12	49
#define PIN_PH13	50
#define PIN_PH14	51
#define PIN_PH15	52
#define PIN_PH16	53
#define PIN_PH17	54
#define PIN_PH18	55
#define PIN_PH19	56
#define PIN_PH20	57
#define PIN_PH21	58
#define PIN_PH22	59
#define PIN_PH23	60
#define PIN_PH24	61
#define PIN_PH25	62
#define PIN_PH26	63
#define PIN_PH27	64

#define PIN_PB18	67
#define PIN_PB19	68
#define PIN_PB20	69
#define PIN_PB21	70
#define PIN_PB22	71
#define PIN_PB23	72

#define PIN_PI19	75	
#define PIN_PI18	76
#define PIN_PI17	77
#define PIN_PI16	78
#define PIN_PG0		79
#define PIN_PG1		80
#define PIN_PG2		81
#define PIN_PG3		82
#define PIN_PG4		83
#define PIN_PG5		84
#define PIN_PG6		85
#define PIN_PG7		86
#define PIN_PG8		87
#define PIN_PG9		88
#define PIN_PG10	89
#define PIN_PG11	90
#define PIN_PE0		91
#define PIN_PE1		92
#define PIN_PE2		93
#define PIN_PE3		94
#define PIN_PE4		95
#define PIN_PE5		96
#define PIN_PE6		97
#define PIN_PE7		98
#define PIN_PE8		99
#define PIN_PE9		100
#define PIN_PE10	101
#define PIN_PE11	102
#define PIN_PI0		103
#define PIN_PI1		104
#define PIN_PI2		105

#define PIN_PI3		107
#define PIN_PB2		108	
#define PIN_PI10	109
#define PIN_PI11	110
#define PIN_PI12	111
#define PIN_PI13	112
#define PIN_PI4		113
#define PIN_PI5		114
#define PIN_PI6		113
#define PIN_PI7		116
#define PIN_PI8		117
#define PIN_PI9		118
#define PIN_PI20	119
#define PIN_PI21	120
#define PIN_PB3		121
#define PIN_PB4		122
#define PIN_PB5		123
#define PIN_PB6		124
#define PIN_PB7		125
#define PIN_PB8		126
#define PIN_PB9		127
#define PIN_PB10	128
#define PIN_PB11	129
#define PIN_PB12	130
#define PIN_PB13	131

#define PIN_PB14	133
#define PIN_PB15	134
#define PIN_PB16	135
#define PIN_PB17	136
#define PIN_PI14	137
#define PIN_PI15	138

#elif defined (BOARD_RASPBERRY_RV2)

#define GPIO0       0 
#define GPIO1       1 
#define GPIO2       2 
#define GPIO3       3 
#define GPIO4       4 
#define GPIO5       5 
#define GPIO6       6 
#define GPIO7       7 
#define GPIO8       8 
#define GPIO9       9 
#define GPIO10      10
#define GPIO11      11
#define GPIO12      12
#define GPIO13      13
#define GPIO14      14
#define GPIO15      15
#define GPIO16      16
#define GPIO17      17
#define GPIO18      18
#define GPIO19      19
#define GPIO20      20
#define GPIO21      21
#define GPIO22      22
#define GPIO23      23
#define GPIO24      24
#define GPIO25      25
#define GPIO26      26
#define GPIO27      27
#define GPIO28      28
#define GPIO29      29
#define GPIO30      30
#define GPIO31      31
#define GPIO32      32
#define GPIO33      33
#define GPIO34      34
#define GPIO35      35
#define GPIO36      36
#define GPIO37      37
#define GPIO38      38
#define GPIO39      39
#define GPIO40      40
#define GPIO41      41
#define GPIO42      42
#define GPIO43      43
#define GPIO44      44
#define GPIO45      45
#define GPIO46      46
#define GPIO47      47
#define GPIO48      48
#define GPIO49      49
#define GPIO50      50
#define GPIO51      51
#define GPIO52      52
#define GPIO53      53

#endif /* BOARD_ITEADUINO_PLUS */

/*
 * Declare functions. See defines of funcitons for more usage.
 */
uint32_t pinMode(uint16_t pin, uint8_t mode);
uint32_t digitalWrite(uint16_t pin, uint8_t val);
uint32_t digitalRead(uint16_t pin);
uint32_t analogWrite(uint16_t pin, uint8_t duty);

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

#ifdef __cplusplus
} // extern "C"
#endif


#endif
