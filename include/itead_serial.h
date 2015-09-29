/**
 * @file itead_serial.h
 *
 * Provide the implementation of Arduino API on Serial
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
#ifndef __ITEAD_SERIAL_H__
#define __ITEAD_SERIAL_H__

#include <stdint.h>

#include <itead_config.h>
#include <itead_print.h>

#ifdef __cplusplus
extern "C"{
#endif



#define		DEV_UART0		0
#define		DEV_UART1		1
#define		DEV_UART2		2
#define		DEV_UART3		3
#define		DEV_UART4		4
#define		DEV_UART5		5
#define		DEV_UART6		6
#define		DEV_UART7		7


uint32_t		Serialbegin(uint32_t dev, uint32_t baud);
uint32_t		Serialend(uint32_t dev);
uint32_t		Serialflush(uint32_t dev);
uint32_t		Serialwrite(uint32_t dev, uint8_t val);
uint32_t		Serialprint(uint32_t dev, const char *string);
uint32_t		Serialprintln(uint32_t dev, const char *string);
uint32_t		Serialavailable(uint32_t dev);
uint8_t			Serialread(uint32_t dev);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

/**
 * Provide methods to manipulate serial port.
 *
 * @ingroup serial
 */
class Serial_  : public Print
{
private:
	uint32_t dev;
	bool started;
	
public:
    Serial_();
	Serial_(uint32_t dev);
	
	operator bool();
	
	int available(void);
	void begin(uint32_t baud_count);
	void end(void);
	void flush(void);
	uint8_t read(void);
	uint8_t readBytes(char *buffer, size_t length);
	virtual size_t write(uint8_t c);

};

/**
 * @addtogroup serial
 * @{
 */

/** For some needs about SoftwareSerial */
typedef Serial_ SoftwareSerial;

/** @} */

/**
 * @ingroup serial
 * @defgroup serial_instance Predefined Instance of class Serial_
 * @details Accroding to different boards, user can use instances below directly,
 *  because these have been defined in Segnix. 
 *
 * @par On Iteaduino Plus
 * - Serial - A reference to Serial4
 * - Serial2 - UART2(RX:PI19, TX:PI18)
 * - Serial3 - UART3(RX:PG7, TX:PG6)
 * - Serial4 - UART4(RX:PG11, TX:PG10)
 * - Serial7 - UART7(RX:PI21, TX:PI20)
 * 
 * @par On ITEAD CORE EVB
 * - Serial - A reference to Serial4
 * - Serial4 - UART4(RX:PG11, TX:PG10)
 *
 * @par On Raspberry Rv2 and Raspberry Pi Model B+
 * - Serial - A reference to Serial0
 * - Serial0 - UART0(RX:GPIO15, TX:GPIO14)
 *
 * @par On BeagleBoneBlack
 * - Serial - A reference to Serial1
 * - Serial1 - UART1(RX:GPIO0_14, TX:GPIO0_15)
 * - Serial2 - UART2(RX:GPIO0_2 , TX:GPIO0_3 )
 * - Serial4 - UART4(RX:GPIO0_30, TX:GPIO0_31)
 *
 */


/* The first device has a aliase compitable with Arduino */
extern Serial_ & Serial;

#ifdef BOARD_ITEADUINO_PLUS
extern Serial_ Serial2;  /* with aliase 'Serial' */
extern Serial_ Serial3;
extern Serial_ Serial4;
extern Serial_ Serial7;
#elif defined (BOARD_ITEAD_CORE_EVB)
extern Serial_ Serial4;
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
extern Serial_ Serial0; /* with aliase 'Serial' */
#elif defined (BOARD_BEAGLEBONEBLACK)
extern Serial_ Serial1;
extern Serial_ Serial2;
extern Serial_ Serial4; 
#endif 

#endif /* __cplusplus */

#endif
