/**
 * @file itead_wire.c
 *
 * Provide the implementation of Arduino API on i2c
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
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <itead_wire.h>
#include <itead_global.h>
#include <itead_utility.h>

#define RXBUFFER_SIZE	(1*1024)
#define TXBUFFER_SIZE	(1*1024)

#define INIT_I2C_DEVICE_TX_BUFFER(dev)	\
{\
	i2c_devices[dev].tindex = 0;\
	memset((void*)i2c_devices[dev].txbuf,0,sizeof(i2c_devices[dev].txbuf));	\
}

#define INIT_I2C_DEVICE_RX_BUFFER(dev)	\
{\
	i2c_devices[dev].rindex = 0;\
	i2c_devices[dev].rnum = 0;\
	memset((void*)i2c_devices[dev].rxbuf,0,sizeof(i2c_devices[dev].rxbuf));\
}

typedef struct I2C_DEVICE_ST {
	int fd;
	char *node;
	
	uint8_t rxbuf[RXBUFFER_SIZE];
	uint32_t rindex;
	uint32_t rnum;
	
	uint8_t txbuf[TXBUFFER_SIZE];
	uint32_t tindex;
	
} I2C_DEVICE;

static I2C_DEVICE i2c_devices[] = {
#if defined(BOARD_ITEADUINO_PLUS) || defined (BOARD_ITEAD_CORE_EVB)
    [1] = {
		.fd	  = -1,
		.node = "/dev/i2c-1" ,
	},
	[2] = {
		.fd	  = -1,
		.node = "/dev/i2c-2" ,
	},
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    [1] = {
		.fd	  = -1,
		.node = "/dev/i2c-1" ,
	},
#elif defined (BOARD_BEAGLEBONEBLACK)
    [1] = {
		.fd	  = -1,
		.node = "" ,
	},
#endif 
};

static uint32_t valid_dev[] = {
#if defined(BOARD_ITEADUINO_PLUS) || defined (BOARD_ITEAD_CORE_EVB)
    DEV_I2C1,
    DEV_I2C2,
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    DEV_I2C1,
#elif defined (BOARD_BEAGLEBONEBLACK)
    DEV_I2C1,
#endif 
    DEV_NONE
};


/*
 * @name	: Wirebegin
 * @desc	: open device node related to dev and init tx/rx buffer.
 * @param	: dev - the device number of I2C
 * @return	: 0 if success, 1 if failed.
 */
uint32_t Wirebegin(uint32_t dev)
{
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
		
	if( i2c_devices[dev].node == NULL) {
		sdkerr("\nIllegal parameter: dev = %d\n",dev);
		return 1;
	}
	
	i2c_devices[dev].fd = open(i2c_devices[dev].node,O_RDWR);
	if( i2c_devices[dev].fd < 0) {
		sdkerr("\nCannot open file:%s\n",i2c_devices[dev].node);
		return 1;
	}
	
	INIT_I2C_DEVICE_TX_BUFFER(dev);
	INIT_I2C_DEVICE_RX_BUFFER(dev);
	return 0;
}

/*
 * @name	: WirebeginTransmission
 * @desc	: Set slave addr and empty the tx buffer of dev.
 * @param	: dev - the device number of I2C
 *			  addr - 7bits address of slave.
 * @return	: 0 - success, 1 - falied.
 */
uint32_t WirebeginTransmission(uint32_t dev, uint8_t addr)
{
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	if (ioctl(i2c_devices[dev].fd, I2C_SLAVE, addr) < 0) {
		sdkerr("\nioctl failed !\n");
		return 1;
	}
	
	INIT_I2C_DEVICE_TX_BUFFER(dev);
	return 0;
}

/*
 * @name	: Wirewrite
 * @desc	: write val to buffer of dev. DO NOT transmit.
 * @param	: dev - the device number of I2C
 * @return	: the number of data writen to txbuffer.
 */
uint32_t Wirewrite(uint32_t dev, uint8_t val)
{
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
	if(i2c_devices[dev].tindex < sizeof(i2c_devices[dev].txbuf)){
		i2c_devices[dev].txbuf[ i2c_devices[dev].tindex++ ] = val;	
		return 1;
	} else {
		sdkerr("\nTxBuffer Overflow.\n");
		return 0;
	}
}

/*
 * @name	: WireendTransmission
 * @desc	: Actually,transmit tx buffer of dev to slave.
 * @param	: dev - the device number of I2C
 * @return	: 0 - success, 1 - failed.
 */
uint32_t WireendTransmission(uint32_t dev)
{
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	if (write(i2c_devices[dev].fd, i2c_devices[dev].txbuf, i2c_devices[dev].tindex)
		!= i2c_devices[dev].tindex) {
		sdkerr("\nWireendTransmission write failed !\n");
		return 1;
	}
	INIT_I2C_DEVICE_TX_BUFFER(dev);
	return 0;
	
}

/*
 * @name	: WirerequestFrom
 * @desc	: Read data from dev with addr. Length of data is count.
 * @param	: dev - the device number of I2C
 * @return	: the number of avaiable data actually.
 */
uint32_t WirerequestFrom(uint32_t dev, uint8_t addr, uint32_t count)
{
	int ret;
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
	if (ioctl(i2c_devices[dev].fd, I2C_SLAVE, addr) < 0) {
		sdkerr("\nioctl failed !\n");
		return 0;
	}
	
	if (count > sizeof(i2c_devices[dev].rxbuf)) {
		sdkerr("\nIllegal Parameter: count=%d too large.\n",count);
		return 0;
	}
	INIT_I2C_DEVICE_RX_BUFFER(dev);
	
	i2c_devices[dev].rnum = read(i2c_devices[dev].fd, i2c_devices[dev].rxbuf,count);
	return i2c_devices[dev].rnum;
}

/*
 * @name	: Wireavailable
 * @desc	: Get the number of data unread in rx buffer.
 * @param	: dev - the device number of I2C
 * @return	: the number of unread data.
 */
uint32_t Wireavailable(uint32_t dev)
{
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
	return i2c_devices[dev].rnum;
}

/*
 * @name	: Wireread
 * @desc	: Read a date type of uint8_t from rx buffer of dev.
 * @param	: dev - the device number of I2C
 * @return	: the first byte in buffer.
 */
uint8_t Wireread(uint32_t dev)
{
	uint8_t ret = 0;
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
	if ( i2c_devices[dev].rnum > 0 ) {
		ret = i2c_devices[dev].rxbuf[i2c_devices[dev].rindex++];
		i2c_devices[dev].rnum--;
		if ( i2c_devices[dev].rnum == 0 ) {
			INIT_I2C_DEVICE_RX_BUFFER(dev);
		}
	} else {
		sdkerr("\nNo more data in rx buffer.\n");
	}
	
	return ret;
}

/*
 * @name	: Wireend
 * @desc	: Close dev , when you do not use it any more.
 * @param	: dev - the device number of I2C
 * @return	: 0 - success, 1 - failed.
 */
uint32_t Wireend(uint32_t dev)
{
	if(!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	close(i2c_devices[dev].fd);
	i2c_devices[dev].fd = -1;
	return 0;
}
	
