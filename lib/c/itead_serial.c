/**
 * @file itead_serial.c
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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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

#include <itead_serial.h>
#include <itead_global.h>
#include <itead_utility.h>

typedef struct SERIAL_PORT_ST {
	int fd;								// file descriptor of node after open
	char *node;							// node name of specific serial port under /dev/
} SERIAL_PORT;

static SERIAL_PORT ports[] = {
#if defined(BOARD_ITEADUINO_PLUS) || defined(BOARD_ITEAD_CORE_EVB)	
    [2] = {
		.fd = -1,
		.node = "/dev/ttyS1",
	},
	[3] = {
		.fd = -1,
		.node = "/dev/ttyS2",
	},
	[4] = {
		.fd = -1,
		.node = "/dev/ttyS3",
	},
	[7] = {
		.fd = -1,
		.node = "/dev/ttyS4",
	},
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    [0] = {
		.fd = -1,
		.node = "/dev/ttyAMA0",
	},
#elif defined (BOARD_BEAGLEBONEBLACK)
    [1] = {
		.fd = -1,
		.node = "/dev/ttyO1",
	},
    [2] = {
		.fd = -1,
		.node = "/dev/ttyO2",
	},
    [4] = {
		.fd = -1,
		.node = "/dev/ttyO4",
	},
#endif 

};

static uint32_t valid_dev[] = {
#ifdef BOARD_ITEADUINO_PLUS
    DEV_UART2,
    DEV_UART3,
    DEV_UART4,
    DEV_UART7,
#elif defined(BOARD_ITEAD_CORE_EVB)    
    DEV_UART4,
#elif defined (BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    DEV_UART0,
#elif defined (BOARD_BEAGLEBONEBLACK)
    DEV_UART1,
    DEV_UART2,
    DEV_UART4,
#endif 
    DEV_NONE
};


/*
 * @name	: Serialbegin
 * @desc	: Init serial port (specified by dev) with baud. After this 
 *			  function, you can read or write data from or to. When you 
 *			  do not use it any more, you'd better to end it by calling 
 *			  Serialend(dev).
 * @param	: dev - serial port number.
 *			  baud - baud rate of dev.
 * @return	: 0 if success, 1 if failed.
 */
uint32_t Serialbegin(uint32_t dev, uint32_t baud)
{
	struct termios _options;
	speed_t _myBaud;
	int     _status;

	if (!vertify_dev(dev,valid_dev)) {
		sdkerr("\nSerialbegin:bad dev=%d\n",dev);
		return 1;
	}
	
	switch (baud)
	{
	    case     50:	_myBaud =     B50 ; break ;
        case     75:	_myBaud =     B75 ; break ;
        case    110:	_myBaud =    B110 ; break ;
        case    134:	_myBaud =    B134 ; break ;
        case    150:	_myBaud =    B150 ; break ;
        case    200:	_myBaud =    B200 ; break ;
        case    300:	_myBaud =    B300 ; break ;
        case    600:	_myBaud =    B600 ; break ;
        case   1200:	_myBaud =   B1200 ; break ;
        case   1800:	_myBaud =   B1800 ; break ;
        case   2400:	_myBaud =   B2400 ; break ;
        case   4800:	_myBaud =   B4800 ; break ;
		case   9600:	_myBaud =   B9600 ; break ;
		case  19200:	_myBaud =  B19200 ; break ;
		case  38400:	_myBaud =  B38400 ; break ;
		case  57600:	_myBaud =  B57600 ; break ;
		case 115200:	_myBaud = B115200 ; break ;
        case 230400:	_myBaud = B230400 ; break ;
		default:
			sdkerr("\nillegal parameter: baud\n");
			return 1;
	}

  	if ((ports[dev].fd = open(ports[dev].node, 
		O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1) {
		sdkerr("\nCannot open file:%s\n",ports[dev].node);
		return 1 ;
	}
    
  	fcntl (ports[dev].fd, F_SETFL, O_RDWR) ;
	tcgetattr (ports[dev].fd, &_options) ;
    cfmakeraw   (&_options) ;
    cfsetispeed (&_options, _myBaud) ;
    cfsetospeed (&_options, _myBaud) ;

    _options.c_cflag |= (CLOCAL | CREAD) ;
    _options.c_cflag &= ~PARENB ;
    _options.c_cflag &= ~CSTOPB ;
    _options.c_cflag &= ~CSIZE ;
    _options.c_cflag |= CS8 ;
    _options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
    _options.c_oflag &= ~OPOST ;

    _options.c_cc [VMIN]  =   0 ;
    _options.c_cc [VTIME] = 100 ;

	tcsetattr (ports[dev].fd, TCSANOW | TCSAFLUSH, &_options) ;

	ioctl (ports[dev].fd,TIOCMGET,&_status);

	_status |= TIOCM_DTR ;
	_status |= TIOCM_RTS ;

	ioctl (ports[dev].fd, TIOCMSET, &_status);
	usleep (10000) ;	

	return 0;
}


/*
 * @name	: Serialflush
 * @desc	: Empty received buffer and transmit buffer.
 * @param	: dev - serial port number.
 * @return	: 0 if success, 1 if failed.
 */
uint32_t Serialflush (uint32_t dev)
{
	if (!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	/* tcflush: if failed,return -1. 0 success */
	if(tcflush(ports[dev].fd, TCIOFLUSH) < 0) {
		return 1;
	} else {
		return 0;
	}
}

/*
 * @name	: Serialend
 * @desc	: close the resource of serial port specified by dev.
 * @param	: dev - serial port number.
 * @return	: 0 if success, 1 if failed.
 */
uint32_t Serialend (uint32_t  dev)
{
	if (!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 1;
	}
	if(close(ports[dev].fd) < 0)
	{
		return 1;
	} else {
		ports[dev].fd = -1;
		return 0;
	}
}

/*
 * @name	: Serialwrite
 * @desc	: Write a val with type of uint8_t to serial port specified by dev.
 * @param	: dev - serial port number.
 *			  val - value to write
 * @return	: the number of bytes written.
 */
uint32_t Serialwrite (uint32_t dev, uint8_t val)
{
	if (!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}

	if(write(ports[dev].fd, &val, 1) < 0)
	{
		return 0;
	} else {
		return 1;
	}
}

/*
 * @name	: Serialprint
 * @desc	: Write a string to serial port specified by dev.
 * @param	: dev - serial port number.
 *			  string - pointer of string to write
 * @return	: the number of bytes written.
 */
uint32_t Serialprint (uint32_t dev, const char *string)
{
	if (!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
	return write(ports[dev].fd, string, strlen (string));
	
}

/*
 * @name	: Serialprintln
 * @desc	: Write a string attached with "\r\n" to serial port specified by dev.
 * @param	: dev - serial port number.
 *			  string - pointer of string to write
 * @return	: the number of bytes written.
 */
uint32_t Serialprintln (uint32_t dev, const char *string)
{
	int nbyte;
	if (!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
  	nbyte = write (ports[dev].fd, string, strlen (string));
	nbyte += write (ports[dev].fd,"\r\n",strlen("\r\n"));
	return nbyte;
}

/*
 * @name	: Serialavailable
 * @desc	: Check the number of unread data in buffer of serial port specified by dev.
 * @param	: dev - serial port number.
 * @return	: the number of unread data.
 */
uint32_t Serialavailable (uint32_t dev)
{
  	uint32_t result;
	if (!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
  	if (ioctl (ports[dev].fd, FIONREAD, &result) == -1) {
		sdkerr("\nSerialavailable failed!\n");
		return 0;
	}
	return result;
}

/*
 * @name	: Serialread
 * @desc	: Read a uint8_t data from serial port specified by dev.
 * @param	: dev - serial port number.
 * @return	: the first byte in buffer if possible.
 */
uint8_t Serialread (uint32_t dev)
{
  	uint8_t data;
	if (!vertify_dev(dev,valid_dev)) {
		sdkerr("\nbad dev=%d\n",dev);
		return 0;
	}
  	if (read (ports[dev].fd, &data, 1) != 1) {
		sdkerr("\nSerialread failed!\n");
		return 0;
  	}
  	return data;
}
