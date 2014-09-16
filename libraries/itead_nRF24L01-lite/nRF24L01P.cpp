/********************************************************************
* File		:	nRF24L01P.cpp
* Desc		:	Manipulate nRF24L01P module
* Author	: 	Zheng Zhiyu <zzy@iteadstudio.com>
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/3/11
* History	:	Version		Modified by		Date		    What
*				v1.0		Zheng Zhiyu		2014/5/24	    Create
********************************************************************/

/*
 * (C) Copyright 2014
 * ITEAD Intelligent Systems Co., Ltd. <http://imall.iteadstudio.com>
 * Zheng Zhiyu <zzy@iteadstudio.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#if 1 // add by wpf
#include <itead.h>
#else
#include "Arduino.h"
#endif
#include "nRF24L01P.h"

//Define the commands for operate the nRF24L01P
#define READ_nRF_REG        0x00  	// Command for read register
#define WRITE_nRF_REG       0x20 	// Command for write register
#define RD_RX_PLOAD     0x61  	// Command for read Rx payload
#define WR_TX_PLOAD     0xA0  	// Command for write Tx payload
#define FLUSH_TX        0xE1 	// Command for flush Tx FIFO
#define FLUSH_RX        0xE2  	// Command for flush Rx FIFO
#define REUSE_TX_PL     0xE3  	// Command for reuse Tx payload
#define NOP             0xFF  	// Reserve

//Define the register address for nRF24L01P
#define CONFIG          0x00  //  Configurate the status of transceiver, mode of CRC and the replay of transceiver status
#define EN_AA           0x01  //  Enable the atuo-ack in all channels
#define EN_RXADDR       0x02  //  Enable Rx Address
#define SETUP_AW        0x03  // Configurate the address width
#define SETUP_RETR      0x04  //  setup the retransmit
#define RF_CH           0x05  // Configurate the RF frequency
#define RF_SETUP        0x06  // Setup the rate of data, and transmit power
#define NRFRegSTATUS    0x07  //
#define OBSERVE_TX      0x08  //
#define CD              0x09  //    //Carrier detect
#define RX_ADDR_P0      0x0A  //  Receive address of channel 0
#define RX_ADDR_P1      0x0B  // Receive address of channel 1
#define RX_ADDR_P2      0x0C  // Receive address of channel 2
#define RX_ADDR_P3      0x0D  // Receive address of channel 3
#define RX_ADDR_P4      0x0E  // Receive address of channel 4
#define RX_ADDR_P5      0x0F  // Receive address of channel 5
#define TX_ADDR         0x10  //       Transmit address
#define RX_PW_P0        0x11  //  Size of receive data in channel 0
#define RX_PW_P1        0x12  //  Size of receive data in channel 1
#define RX_PW_P2        0x13  //  Size of receive data in channel 2
#define RX_PW_P3        0x14  //  Size of receive data in channel 3
#define RX_PW_P4        0x15  // Size of receive data in channel 4
#define RX_PW_P5        0x16  //  Size of receive data in channel 5
#define FIFO_STATUS     0x17  // FIFO Status
///**************************************************************************************


nRF24L01P::nRF24L01P(uint16_t ce, uint16_t cs, SPIClass & SPI) 
{
    int i;
    nRF24L01_Freq = 0;
    nRF24L01_power_rate = 0;
    for(i=0; i<Buffer_Size; i++) {
        TxBuf[i] = 0;
        RxBuf[i] = 0;
    }
    for(i=0; i<ADR_WIDTH; i++) {
        TX_ADDRESS[i] = 0xE7;
        RX_ADDRESS[i] = 0xE7;
    }
    
    _SPI =  &SPI;
    spi_cs = cs;
    this->ce = ce;
}



//Define the layer3 functions

void nRF24L01P::nRF24L01_HW_Init(void)
{
	_SPI->begin();
	_SPI->setDataMode(0); /* Is It OK ??? */
    _SPI->setBitOrder(MSBFIRST);
    _SPI->setClockDivider(SPI_CLOCK_DIV32);
	pinMode(spi_cs,OUTPUT);
    pinMode(ce,OUTPUT);
    nRF24L01_CE_L();
    
    /* PWR_DOWN */
    SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x00);
    delay(5);
    /* PWR_UP */
    SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x02); // add by wpf 
    delay(5);
}

void nRF24L01P::nRF24L01_Set_TX_Address(	unsigned char A,
								unsigned char B,
								unsigned char C,
								unsigned char D,
								unsigned char E)
{
	TX_ADDRESS[0] = A;
	TX_ADDRESS[1] = B;
	TX_ADDRESS[2] = C;
	TX_ADDRESS[3] = D;
	TX_ADDRESS[4] = E;
}
void nRF24L01P::nRF24L01_Set_RX_Address(	unsigned char A,
								unsigned char B,
								unsigned char C,
								unsigned char D,
								unsigned char E)
{
	RX_ADDRESS[0] = A;
	RX_ADDRESS[1] = B;
	RX_ADDRESS[2] = C;
	RX_ADDRESS[3] = D;
	RX_ADDRESS[4] = E;
}

unsigned char nRF24L01P::nRF24L01_Config(unsigned char freq, unsigned char power, unsigned char Rate)
{
	nRF24L01_Freq = 0;
	nRF24L01_power_rate = 0;
    #if 0 // add by wpf
	if((freq>125)&&(freq<0))
    #else
    if((freq>125)||(freq<0))
    #endif
		return 0;
	else
		nRF24L01_Freq = freq;

	if (P0dBm == power)
		nRF24L01_power_rate|=0x06;
	else if (Pm6dBm == power)
		nRF24L01_power_rate|=0x04;
	else if (Pm12dBm == power)
		nRF24L01_power_rate|=0x02;
	else if (Pm18dBm == power)
		nRF24L01_power_rate|=0x00;
	else 
		return 0;

	if (R2mbps == Rate)
		{nRF24L01_power_rate|=0x08;}
	else if (Rate == R1mbps)
		{nRF24L01_power_rate|=0x00;}
	else if (Rate == R250kbps)
		nRF24L01_power_rate|=0x20;
	else
		return 0;

	return 1;
	
}

void nRF24L01P::RX_Mode(void)
{
    unsigned char buf[5]={0};

    nRF24L01_CE_L();
    nRF24L01_Delay_us(1);
    
	SPI_Read_Buf(TX_ADDR, buf, ADR_WIDTH);
	SPI_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH);

	SPI_WR_Reg(WRITE_nRF_REG + EN_AA, 0);
	SPI_WR_Reg(WRITE_nRF_REG + EN_RXADDR, 0x01);
	SPI_WR_Reg(WRITE_nRF_REG + SETUP_RETR, 0x1a);
	SPI_WR_Reg(WRITE_nRF_REG + RF_CH,nRF24L01_Freq);
	SPI_WR_Reg(WRITE_nRF_REG + RX_PW_P0, RX_PLOAD_WIDTH);
	SPI_WR_Reg(WRITE_nRF_REG + RF_SETUP, nRF24L01_power_rate);

	SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x03);
    nRF24L01_CE_H();
	nRF24L01_Delay_us(200);
    
}

void nRF24L01P::TX_Mode(void)
{


    nRF24L01_CE_L();
    nRF24L01_Delay_us(1);
    
	SPI_Write_Buf(WRITE_nRF_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH);
	SPI_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH);

	SPI_WR_Reg(WRITE_nRF_REG + EN_AA, 0);
	SPI_WR_Reg(WRITE_nRF_REG + EN_RXADDR, 0x01);
	SPI_WR_Reg(WRITE_nRF_REG + SETUP_RETR, 0x1a);
	SPI_WR_Reg(WRITE_nRF_REG + RF_CH,nRF24L01_Freq);
	SPI_WR_Reg(WRITE_nRF_REG + RF_SETUP,  nRF24L01_power_rate);
	SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x02);

    nRF24L01_CE_H();
	nRF24L01_Delay_us(200);
}

void nRF24L01P::nRF24L01_TxPacket(unsigned char * tx_buf)
{

	SPI_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, TX_ADDRESS, ADR_WIDTH);
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);


}

unsigned char nRF24L01P::nRF24L01_RxPacket(unsigned char* rx_buf)
{unsigned char flag=0;
 unsigned char status;

	status=SPI_RD_Reg(NRFRegSTATUS);
	if(status & 0x40)
	{
		 SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
		 flag =1;
	}
	SPI_WR_Reg(WRITE_nRF_REG+NRFRegSTATUS, status);
	return flag;
}



//Define the layer2 functions
unsigned char nRF24L01P::SPI_RD_Reg(unsigned char reg)
{
	unsigned char reg_val;

	nRF24L01_SPI_NSS_L();                // CSN low, initialize SPI communication...
	nRF24L01_SPI_Send_Byte(reg);            // Select register to read from..
	reg_val = nRF24L01_SPI_Send_Byte(0);    // ..then read register value
	nRF24L01_SPI_NSS_H();                // CSN high, terminate SPI communication

	return(reg_val);        // return register value
}

unsigned char nRF24L01P::SPI_WR_Reg(unsigned char reg, unsigned char value)
{
	unsigned char status;

	nRF24L01_SPI_NSS_L();                  // CSN low, init SPI transaction
	status = nRF24L01_SPI_Send_Byte(reg);// select register
	nRF24L01_SPI_Send_Byte(value);             // ..and write value to it..
	nRF24L01_SPI_NSS_H();                   // CSN high again

	return(status);            // return nRF24L01 status unsigned char
}

unsigned char nRF24L01P::SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len)
{
	unsigned int status,i;

	nRF24L01_SPI_NSS_L();                    		// Set CSN low, init SPI tranaction
	status = nRF24L01_SPI_Send_Byte(reg);       		// Select register to write to and read status unsigned char

  for(i=0;i<Len;i++)
  {
     pBuf[i] = nRF24L01_SPI_Send_Byte(0);
  }

	nRF24L01_SPI_NSS_H();

	return(status);                    // return nRF24L01 status unsigned char
}

unsigned char nRF24L01P::SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len)
{
	unsigned int status,i;

	nRF24L01_SPI_NSS_L();
	status = nRF24L01_SPI_Send_Byte(reg);
	for(i=0; i<Len; i++) //
	{
		nRF24L01_SPI_Send_Byte(*pBuf);
		 pBuf ++;
	}
	nRF24L01_SPI_NSS_H();    
	return(status);   
}



//Define the layer1 functions
unsigned char nRF24L01P::nRF24L01_SPI_Send_Byte(unsigned char dat)
{
  return _SPI->transfer(dat);
}

void nRF24L01P::nRF24L01_CE_H(void)
{
	digitalWrite(ce,HIGH);
}

void nRF24L01P::nRF24L01_CE_L(void)
{
	digitalWrite(ce,LOW);
}

void nRF24L01P::nRF24L01_SPI_NSS_H(void)
{
	digitalWrite(spi_cs,HIGH);
}

void nRF24L01P::nRF24L01_SPI_NSS_L(void)
{
	digitalWrite(spi_cs,LOW);
}

//Define the other function

void nRF24L01P::nRF24L01_Delay_us(unsigned long n) 
{
    delayMicroseconds(n);
}


