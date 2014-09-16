/**
 * @file itead_spi.c
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
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <linux/spi/spidev.h>
#include <errno.h>

#include <itead_spi.h>
#include <itead_global.h>
#include <itead_utility.h>


/* comment the line blow if you don't want see debug info */
//#define DEBUG
#include <itead_debug.h>

#define SPI_RECEIVE_ALL_ACTIVE_      (0x20) 

typedef struct SPI_DEVICE_ST {
	int fd;						// file descriptor of spi device node
	char *node;					// /dev/spidev%d.%d 
	uint8_t rxbuf[1];			// buffer of receive
	uint8_t txbuf[1];  			// buffer of transmit
	struct spi_ioc_transfer tr;	// pass to ioctl
} SPI_DEVICE;

/*
 * all spi_devices is here.
 * Importantly,node is the device node of spi devices.
 */
static SPI_DEVICE spi_devices[] = {
#ifdef BOARD_ITEADUINO_PLUS
   [0] = {
		.node	= "/dev/spidev0.0",
	},
#elif defined (BOARD_RASPBERRY_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
   [0] = {
		.node	= "/dev/spidev0.0",
	},
#endif /* BOARD_ITEADUINO_PLUS */
};

static uint32_t valid_dev[] = {
#ifdef BOARD_ITEADUINO_PLUS
    DEV_SPI0,
#elif defined (BOARD_RASPBERRY_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    DEV_SPI0,
#endif /* BOARD_ITEADUINO_PLUS */
    DEV_NONE
};

/*
 * @name	: SPIbegin
 * @desc	: open device node related to dev and init struct spi_ioc_transfer.
 * @param	: dev - the device number of SPI
 * @return	: 0 if success, 1 if failed.
 */
uint32_t SPIbegin(uint32_t dev)
{
    int ret = 0;
    int max_speed = 0;
	debug("\nSPIbegin\n");
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	spi_devices[dev].fd = open(spi_devices[dev].node , O_RDWR);
    if(spi_devices[dev].fd < 0) {
        sdkerr("\nopen spidev failed\n");
        return 1;
    }

    ret = ioctl(spi_devices[dev].fd, 
		SPI_IOC_RD_MAX_SPEED_HZ, 
		&spi_devices[dev].tr.speed_hz);
	
    if (ret == -1) {
	    sdkerr("\ncan't get max speed hz\n");
		return 1;
	}
	
	spi_devices[dev].tr.tx_buf = (unsigned long)spi_devices[dev].txbuf;
	spi_devices[dev].tr.rx_buf = (unsigned long)spi_devices[dev].rxbuf;
	spi_devices[dev].tr.bits_per_word = 8;
	spi_devices[dev].tr.delay_usecs = 0;
	spi_devices[dev].tr.len = sizeof(spi_devices[dev].rxbuf);
	
    debug("\nmax speed: %d Hz (%d KHz)\n", spi_devices[dev].tr.speed_hz, spi_devices[dev].tr.speed_hz/1000);

	return 0;
}


/*
 * @name	: SPIsetBitOrder
 * @desc	: determine the first bit of sending data via dev.
 * @param	: dev - the device number of SPI
 *			  order - LSBFIRST or MSBFIRST
 * @return	: 0 if success, 1 if failed.
 */
uint32_t SPIsetBitOrder(uint32_t dev, uint8_t order)
{
    uint8_t bitorder = order;
	int ret = 0;
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	if( (order != LSBFIRST) && (order != MSBFIRST)) {
		sdkerr("\nillegal parameter: order\n");
		return 1;
	}
	
     ret = ioctl(spi_devices[dev].fd, SPI_IOC_WR_LSB_FIRST, &bitorder);
     if (ret == -1) {
	 	sdkerr("\ncan't set bits order\n");
         sdkerr("NO.=%d,%s\n",errno,strerror(errno));
		return 1;
	 }
	 
	 ret = ioctl(spi_devices[dev].fd, SPI_IOC_RD_LSB_FIRST, &bitorder);
     if (ret == -1) {
	 	sdkerr("\ncan't get bits order\n");
		return 1;
	 }
	 
	 if ( bitorder == LSBFIRST) {
	 	debug("\nbit order : lsb first\n");	
	 } else if( bitorder == MSBFIRST) {
	 	debug("\nbit order : msb first\n");
	 } else {
	 	sdkerr("\nget bit order err!\n");
		return 1;
	 }
	 
	 return 0;
}

/*
 * @name	: SPIsetDataMode
 * @desc	: Sets the SPI data mode: that is, clock polarity and phase
 * @param	: dev - the device number of SPI
 * 			  mode - SPI_MODE0/SPI_MODE1/SPI_MODE2/SPI_MODE3
 * @return	: 0 if success, 1 if failed.
 */
uint32_t SPIsetDataMode(uint32_t dev, uint8_t mode)
{
    int ret = 0;
    uint8_t smode = 0;
   	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
    ret = ioctl(spi_devices[dev].fd, SPI_IOC_RD_MODE, &smode);
    if (ret == -1) {
		sdkerr("\ncan't get spi mode\n");
		return  1;
	}
	/* set polarity and phase */
    smode &= ~0x3;
    smode |= (mode&0x3);
    
	/* !!! open Full-duplex synchronous !!! */
    #ifdef  BOARD_ITEADUINO_PLUS // add by wpf
	smode |= SPI_RECEIVE_ALL_ACTIVE_;
	#endif
	
	ret = ioctl(spi_devices[dev].fd, SPI_IOC_WR_MODE, &smode);
   	if (ret == -1) {
		sdkerr("\ncan't set spi mode\n");
		return  1;
	}
	
    ret = ioctl(spi_devices[dev].fd, SPI_IOC_RD_MODE, &smode);
    if (ret == -1) {
		sdkerr("\ncan't get spi mode\n");
		return  1;
	}
    debug("spi mode: 0x%x\n", smode);
	return 0;
}

#if 1 /* may be useless */
/*
 * @name	: SPIsetClockDivider
 * @desc	: modify spi frequency of dev's spi clock baseon max_speed_hz
 * @param	: dev - the device number of SPI
 *			  divider - SPI_CLOCK_DIVn (n=1,2,4,8,16,32,64,128)
 * @return	: 0 if success, 1 if failed.
 * @notes	: this function may be useless for now.
 */
uint32_t SPIsetClockDivider(uint32_t dev, uint16_t divider)
{
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	if( divider ) {
        #if 0 // add by wpf
		spi_devices[dev].tr.speed_hz |= divider;	
        #else
        spi_devices[dev].tr.speed_hz /= divider;	
        #endif
	} else {
		sdkerr("\nset clock divider err: cannot be 0\n");
		return 1;
	}
	debug("\nset clock divider success:0x%X\n",divider);
	
    return 0;
}
#endif

/*
 * @name	: SPItransfer
 * @desc	: transmit val by dev and receive data from dev simultaneously.
 * @param	: dev - the device number of SPI
 *			  val - value need to send.
 * @return	: received data 
 */
uint8_t SPItransfer(uint32_t dev, uint8_t val)
{
	int ret=0;
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
  	spi_devices[dev].txbuf[0] = val;
	
    ret = ioctl(spi_devices[dev].fd, SPI_IOC_MESSAGE(1), &spi_devices[dev].tr);
    if (ret < 0) {
		sdkerr("\bcan't send spi message\n");
		return 0;
	}
    return spi_devices[dev].rxbuf[0];
}

/*
 * @name	: SPIend
 * @desc	: close spi device specified by dev.
 * @param	: dev - the device number of SPI
 * @return	: 0 - success, 1 - failed.
 */
uint32_t SPIend(uint32_t dev)
{
	debug("\nSPIend\n");
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	close(spi_devices[dev].fd);
	return 0;
}
