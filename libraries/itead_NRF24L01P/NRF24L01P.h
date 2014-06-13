/********************************************************************
* File		:	NRF24L01P.h
* Desc		:	Manipulate nRF24L01P module
* Author	: 	Wu Pengfei <pengfei.wu@itead.cc>
* Company	:	ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
* Date		:	2014/3/11
* History	:	Version		Modified by		Date		    What
*				v1.0		Wu Pengfei		      2014/6/13	    Create
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
#ifndef __NRF24L01P_LIB_H__ 
#define __NRF24L01P_LIB_H__ 

#include <itead.h>
#include <inttypes.h>

#define TXRX_ADDR_WIDTH      5
#define PAYLOAD_MAX_WIDTH   32
#define TXRX_BUFFER_SIZE    PAYLOAD_MAX_WIDTH
#define USER_DATA_WIDTH     24

//Define RF power value
#define PWR_0dBm            1
#define PWR_n6dBm           2
#define PWR_n12dBm          3
#define PWR_n18dBm          4

//define RF rate
#define AIR_RATE_2Mbps      1
#define AIR_RATE_1Mbps      2
#define AIR_RATE_256Kbps    3

/* Define Command Name */
#define CMD_R_REGISTER(addr)    ((0x00)+(addr))     /* 000A AAAA */
#define CMD_W_REGISTER(addr)    ((0x20)+(addr))     /* 001A AAAA */
#define CMD_R_RX_PAYLOAD        0x61                /* 0110 0001 */
#define CMD_W_TX_PAYLOAD        0xA0                /* 1010 0000 */
#define CMD_FLUSH_TX            0xE1                /* 1110 0001 */
#define CMD_FLUSH_RX            0xE2                /* 1110 0010 */
#define CMD_REUSE_TX_PL         0xE3                /* 1110 0011 */
#define CMD_R_RX_PL_WID         0x60                /* 0110 0000 */
#define CMD_W_ACK_PAYLOAD(ppp)  ((0xA8)+(ppp))      /* 1010 1PPP */
#define CMD_W_TX_PAYLOAD_NOACK  0xB0                /* 1011 0000 */
#define CMD_R_STATUS            0xFF                /* 1111 1111 */


/* Define Register Name with Address */
#define REG_CONFIG         0x00 
#define REG_EN_AA          0x01 
#define REG_EN_RXADDR      0x02 
#define REG_SETUP_AW       0x03 
#define REG_SETUP_RETR     0x04 
#define REG_RF_CH          0x05 
#define REG_RF_SETUP       0x06 
#define REG_STATUS         0x07 
#define REG_OBSERVE_TX     0x08 
#define REG_RPD            0x09 
#define REG_RX_ADDR_P0     0x0A 
#define REG_RX_ADDR_P1     0x0B 
#define REG_RX_ADDR_P2     0x0C 
#define REG_RX_ADDR_P3     0x0D 
#define REG_RX_ADDR_P4     0x0E 
#define REG_RX_ADDR_P5     0x0F 
#define REG_TX_ADDR        0x10 
#define REG_RX_PW_P0       0x11 
#define REG_RX_PW_P1       0x12 
#define REG_RX_PW_P2       0x13 
#define REG_RX_PW_P3       0x14 
#define REG_RX_PW_P4       0x15 
#define REG_RX_PW_P5       0x16 
#define REG_FIFO_STATUS    0x17 
#define REG_DYNPD          0x1C 
#define REG_FEATURE        0x1D

/* Reset value for all registers */
#define RST_CONFIG         0x08 
#define RST_EN_AA          0x3F 
#define RST_EN_RXADDR      0x03 
#define RST_SETUP_AW       0x03 
#define RST_SETUP_RETR     0x03 
#define RST_RF_CH          0x02 
#define RST_RF_SETUP       0x0E 
#define RST_STATUS         0x7E     /* Contian read only bits, clear interrupt pending bits */ 
#define RST_OBSERVE_TX     0x00     /* Read Only, packet loss statistic, ignore */ 
#define RST_RPD            0x00     /* Read Only, Recieved Power Detector, ignore */ 
#define RST_RX_ADDR_P0     0xE7     /* 5 Bytes */ 
#define RST_RX_ADDR_P1     0xC2     /* 5 Bytes */ 
#define RST_RX_ADDR_P2     0xC3     /* LSByte  */ 
#define RST_RX_ADDR_P3     0xC4     /* LSByte  */ 
#define RST_RX_ADDR_P4     0xC5     /* LSByte  */ 
#define RST_RX_ADDR_P5     0xC6     /* LSByte  */ 
#define RST_TX_ADDR        0xE7     /* 5 Bytes */
#define RST_RX_PW_P0       0x00 
#define RST_RX_PW_P1       0x00 
#define RST_RX_PW_P2       0x00 
#define RST_RX_PW_P3       0x00 
#define RST_RX_PW_P4       0x00 
#define RST_RX_PW_P5       0x00 
#define RST_FIFO_STATUS    0x11     /* Read Only, Empty TX_FIFO and RX_FIFO, clear interrupt pending bits */ 
#define RST_DYNPD          0x00 
#define RST_FEATURE        0x00


/* Define Class NRF24L01P */
class NRF24L01P {
public:
    NRF24L01P(void);
    void begin(
        uint16_t ce,        uint16_t spi_cs, 
        uint8_t pwr,        uint8_t rate,       uint8_t rf_ch,
        uint8_t *rx_addr
    );
    void begin(
        SPIClass &SPI,
        uint16_t ce,        uint16_t spi_cs, 
        uint8_t pwr,        uint8_t rate,       uint8_t rf_ch,
        uint8_t *rx_addr
    );
    void end(void); /* Enter POWER DOWN mode */
        
    int8_t send_packet(uint8_t *dest, uint8_t *buffer);
    int8_t recv_packet(uint8_t *src, uint8_t *buffer);

    
private:
    /* Layer 3: Inner functional methods */
    void set_payload_width(uint8_t width);
    void set_tx_addr(uint8_t *tx_addr);
    void set_rx_addr(uint8_t *rx_addr);
    
    void set_to_PTX(void); /* PRIM_RX = 0 */
    void set_to_PRX(void); /* PRIM_RX = 1 */
    void set_to_POWER_UP(void); /* PWR_UP = 1,delay 10ms */
    void set_to_POWER_DOWN(void); /* PWR_UP = 0 delay 1ms */
    
    void clear_interrupt_RX_DR(void);
    void clear_interrupt_TX_DS(void);
    void clear_interrupt_MAX_RT(void);
    void clear_interrupt_all(void);


    /* Layer 2: command functions */
    uint8_t r_register(uint8_t addr,uint8_t *buffer,uint8_t len);
    uint8_t w_register(uint8_t addr,uint8_t *buffer,uint8_t len);
    uint8_t r_rx_payload(uint8_t *buffer,uint8_t len);
    uint8_t w_tx_payload(uint8_t *buffer,uint8_t len);
    uint8_t flush_tx(void);
    uint8_t flush_rx(void);
    uint8_t reuse_tx_pl(void);
    uint8_t r_rx_pl_wid(uint8_t *width);
    uint8_t w_ack_payload(uint8_t ppp,uint8_t *buffer,uint8_t len);
    uint8_t w_tx_payload_noack(uint8_t *buffer,uint8_t len);
    uint8_t r_status(void);
        
    /* Layer 1: Hardware Functions */
    void set_ce_high(void);
    void set_ce_low(void);
    
    void spi_cs_high(void);
    void spi_cs_low(void);
    
    uint8_t spi_transfer(uint8_t data);

private:
    uint8_t rx_addr[TXRX_ADDR_WIDTH]; /* RX Data Pipe0 Address (Unique and Constant) */
    uint8_t tx_addr[TXRX_ADDR_WIDTH]; /* TX Address (Variable,Changed) */
    
    uint8_t rx_buffer[TXRX_BUFFER_SIZE]; /* Inner buffer for RX_FIFO */
    uint8_t tx_buffer[TXRX_BUFFER_SIZE]; /* Inner buffer for TX_FIFO */

    SPIClass *_SPI;
    uint16_t spi_cs;    /* spi chip select pin number */
    uint16_t ce;        /* RX/TX Mode chip enable pin number */
};


#endif
