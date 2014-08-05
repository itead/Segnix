/**
 * @file itead_spi.h
 *
 * Provide the implementation of Arduino API on SPI
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
#ifndef __ITEAD_SPI_H__
#define __ITEAD_SPI_H__

#include <stdint.h>
#include <itead_config.h>

#ifdef __cplusplus
extern "C"{
#endif

/* Only define number of devices */
#define DEV_SPI0		0
#define DEV_SPI1		1
#define DEV_SPI2		2
#define DEV_SPI3		3

/*
 * Mode		 	Clock Polarity (CPOL)		Clock Phase (CPHA)
 * SPI_MODE0		0							0
 * SPI_MODE1		0							1
 * SPI_MODE2		1							0
 * SPI_MODE3		1							1
 */
#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

/*
 * These defines below can be pass to SPIsetClockDivider
 * to modify the clock of spi_clk. 
 */
#define SPI_CLOCK_DIV1   0x01
#define SPI_CLOCK_DIV2   0x02
#define SPI_CLOCK_DIV4   0x04
#define SPI_CLOCK_DIV8   0x08
#define SPI_CLOCK_DIV16  0x10
#define SPI_CLOCK_DIV32  0x20
#define SPI_CLOCK_DIV64  0x40
#define SPI_CLOCK_DIV128 0x80

/*
 * You can determine the first bit to send by spi.
 */
#define LSBFIRST 	1
#define MSBFIRST 	0

uint32_t		SPIbegin(uint32_t dev);
uint32_t		SPIend(uint32_t dev);

#if 1	/* may be useless */
uint32_t		SPIsetClockDivider(uint32_t dev, uint16_t divider);
#endif

uint32_t		SPIsetDataMode(uint32_t dev, uint8_t mode);
uint32_t		SPIsetBitOrder(uint32_t dev, uint8_t order);
uint8_t			SPItransfer(uint32_t dev, uint8_t val);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
class SPIClass 
{
private:
	uint32_t dev;

public:
    SPIClass(void);
	SPIClass(uint32_t dev);
	
	void begin();
	void end();
	void setBitOrder(uint8_t order);
	void setDataMode(uint8_t mode);
	void setClockDivider(uint8_t divider);
	uint8_t transfer(uint8_t val);

};

/* The first device has a aliase compitable with Arduino */
extern SPIClass & SPI;
#ifdef BOARD_ITEADUINO_PLUS
extern SPIClass SPI0;
#elif defined (BOARD_RASPBERRY_RV2)
extern SPIClass SPI0;
#endif /* BOARD_ITEADUINO_PLUS */

#endif

#endif
