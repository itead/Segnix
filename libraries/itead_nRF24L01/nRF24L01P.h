/********************************************************************
* File		:	nRF24L01P.h
* Desc		:	Manipulate nRF24L01P module
* Author	: 	Zheng Zhiyu <zzy@iteadstudio.com>
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/3/11
* History	:	Version		Modified by		Date		    What
*                        v1.0            Zheng Zhiyu            2014/5/24    create
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


#ifndef __NRF24L01P_H__
#define __NRF24L01P_H__

#if 1 // add by wpf
#include <itead.h>
#else
#include <Arduino.h>
#include <SPI.h>
#endif

#define ADR_WIDTH 5
#define RX_PLOAD_WIDTH 32
#define TX_PLOAD_WIDTH 32
#define Buffer_Size 32

//Define RF power value
#define P0dBm 0
#define Pm6dBm 1
#define Pm12dBm 2
#define Pm18dBm 3

//#define RF rate
#define R2mbps 0
#define R1mbps 1
#define R250kbps 3


class nRF24L01P { 
public:    
    nRF24L01P(uint16_t ce, uint16_t cs, SPIClass & SPI);

    // Layer 1 functions declaration
    void nRF24L01_HW_Init(void);
    void nRF24L01_Set_TX_Address(unsigned char A,unsigned char B,unsigned char C,unsigned char D,unsigned char E);
    void nRF24L01_Set_RX_Address(unsigned char A,unsigned char B,unsigned char C,unsigned char D,unsigned char E);
    unsigned char nRF24L01_Config(unsigned char freq,unsigned char power,unsigned char rate);
    void RX_Mode(void);
    void TX_Mode(void);
    void nRF24L01_TxPacket(unsigned char * tx_buf);
    unsigned char nRF24L01_RxPacket(unsigned char* rx_buf);


private:
    // Layer 2 functions declaration
    unsigned char SPI_RD_Reg(unsigned char reg);
    unsigned char SPI_WR_Reg(unsigned char reg, unsigned char value);
    unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len);
    unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned char Len);
    
    //Define the layer1 functions
    unsigned char nRF24L01_SPI_Send_Byte(unsigned char dat);
    void nRF24L01_SPI_NSS_H(void);    
    void nRF24L01_SPI_NSS_L(void);
    
    void nRF24L01_CE_H(void);
    void nRF24L01_CE_L(void);
    //Define the other function
    void nRF24L01_Delay_us(unsigned long n);
    
private:
    SPIClass *_SPI;
    uint16_t spi_cs;
    uint16_t ce;
    unsigned char TxBuf[Buffer_Size];
    unsigned char RxBuf[Buffer_Size];
    
    unsigned char nRF24L01_Freq;
    unsigned char nRF24L01_power_rate;
    
    //define the initial Address
    unsigned char  TX_ADDRESS[ADR_WIDTH];
    unsigned char  RX_ADDRESS[ADR_WIDTH];
};




#endif
