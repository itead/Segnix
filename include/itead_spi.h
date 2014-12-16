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

/**
 * @addtogroup spi
 * @{
 */

#define SPI_MODE0 0x00  /**< CPOL = 0, CPHA = 0 */
#define SPI_MODE1 0x01  /**< CPOL = 0, CPHA = 1 */
#define SPI_MODE2 0x02  /**< CPOL = 1, CPHA = 0 */
#define SPI_MODE3 0x03  /**< CPOL = 1, CPHA = 1 */

#define SPI_CLOCK_DIV1   0x01   /**< Clock devider = 1, only passed to SPIsetClockDivider */
#define SPI_CLOCK_DIV2   0x02   /**< Clock devider = 2, only passed to SPIsetClockDivider */
#define SPI_CLOCK_DIV4   0x04   /**< Clock devider = 4, only passed to SPIsetClockDivider */
#define SPI_CLOCK_DIV8   0x08   /**< Clock devider = 8, only passed to SPIsetClockDivider */
#define SPI_CLOCK_DIV16  0x10   /**< Clock devider = 16, only passed to SPIsetClockDivider */
#define SPI_CLOCK_DIV32  0x20   /**< Clock devider = 32, only passed to SPIsetClockDivider */
#define SPI_CLOCK_DIV64  0x40   /**< Clock devider = 64, only passed to SPIsetClockDivider */
#define SPI_CLOCK_DIV128 0x80   /**< Clock devider = 128, only passed to SPIsetClockDivider */

#define LSBFIRST 	1   /**< LSB first passed to SPIsetBitOrder */
#define MSBFIRST 	0   /**< MSB first passed to SPIsetBitOrder */

/** @} */

uint32_t		SPIbegin(uint32_t dev);
uint32_t		SPIend(uint32_t dev);

uint32_t		SPIsetClockDivider(uint32_t dev, uint16_t divider);
uint32_t		SPIsetDataMode(uint32_t dev, uint8_t mode);
uint32_t		SPIsetBitOrder(uint32_t dev, uint8_t order);
uint8_t			SPItransfer(uint32_t dev, uint8_t val);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

/**
 * Provide simple methods to manipulate SPI bus. 
 * 
 * @par An example for using SPIClass
 * We assume that SPI is an instance of class SPIClass.
 * @code
 *      uint8_t receive_data;
 *      // Step 1: init spi
 *      SPI.begin();
 *      // Step 2: configure
 *      SPI.setBitOrder(MSBFIRST);
 *      SPI.setDataMode(SPI_MODE0);
 *      SPI.ClockDivider(SPI_CLOCK_DIV16);
 *      // Step 3: transfer
 *      receive_data = transfer(0x55);
 *      ...
 *      // Step 4: close it
 *      SPI.end();
 * @endcode
 *
 * @ingroup spi
 */
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

/**
 * @ingroup spi
 * @defgroup spi_instance Predefined Instance of class SPIClass
 * @details Accroding to different boards, user can use instances below directly,
 *  because these have been defined in Segnix. 
 * 
 * @par On Iteaduino Plus
 * - SPI - A reference to SPI0
 * - SPI0 - SPI0(CS:PI10, SCK:PI11, MOSI:PI12, MISO:PI13)
 * 
 * @par On ITEAD CORE EVB
 * - SPI - A reference to SPI0
 * - SPI0 - SPI0(CS0:PI10, CS1:PI14, SCK:PI11, MOSI:PI12, MISO:PI13)
 *
 * @par On Raspberry Rv2 and Raspberry Pi Model B+
 * - SPI - A reference to SPI0
 * - SPI0 - SPI0(CE0:GPIO8, CE1:GPIO7, SCK:GPIO11, MOSI:GPIO10, MISO:GPIO9)
 *
 * @par On BeagleBoneBlack
 * - SPI - A reference to SPI1
 * - SPI1 - SPI1(CS0:GPIO3_17, SCK:GPIO3_14, MOSI:GPIO3_16, MISO:GPIO3_15)
 */

extern SPIClass & SPI;

#if defined(BOARD_ITEADUINO_PLUS) || defined (BOARD_ITEAD_CORE_EVB)
extern SPIClass SPI0;
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
extern SPIClass SPI0;
#elif defined (BOARD_BEAGLEBONEBLACK)
extern SPIClass SPI1;
#endif 

#endif

#endif
