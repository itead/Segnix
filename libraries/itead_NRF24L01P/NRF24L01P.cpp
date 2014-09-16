/********************************************************************
* File		:	NRF24L01P.cpp
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
#include "NRF24L01P.h"

NRF24L01P::NRF24L01P(void)
{
    uint8_t i;
    for(i=0;i<TXRX_ADDR_WIDTH;i++){
        tx_addr[i] = RST_TX_ADDR;
        rx_addr[i] = RST_RX_ADDR_P0;
    }
    _SPI = &SPI;
}


/* ***************************************************************************
* nRF24L01P User API
* ***************************************************************************/
void NRF24L01P::begin(
            SPIClass &SPI,
            uint16_t ce,        uint16_t spi_cs, 
            uint8_t pwr,        uint8_t rate,       uint8_t rf_ch,  uint8_t *rx_addr )
{
    _SPI = &SPI;
    begin(ce, spi_cs,pwr, rate, rf_ch, rx_addr);
}

void NRF24L01P::begin(
        uint16_t ce,        uint16_t spi_cs, 
        uint8_t pwr,        uint8_t rate,       uint8_t rf_ch,  uint8_t *rx_addr)
{
    uint8_t val;
    uint8_t val5[5];
    
    this->ce = ce;
    this->spi_cs = spi_cs;
    
    pinMode(ce,OUTPUT);
    set_ce_low();
    delay(10); 
    /* Now, stdby1 or stdby2 
        */
    
    /* init spi */
    _SPI->begin();
	_SPI->setDataMode(SPI_MODE0); 
    _SPI->setBitOrder(MSBFIRST);
    _SPI->setClockDivider(SPI_CLOCK_DIV32);
    
	pinMode(spi_cs,OUTPUT);
    spi_cs_high();
    
    
    /* Reset all registers to value after Power on */
    
    /* Into POWER DOWN mode */
    val = RST_CONFIG;
    w_register(REG_CONFIG,&val,sizeof(val)); 
    
    val = RST_EN_AA;
    w_register(REG_EN_AA,&val,sizeof(val));
    
    val = RST_EN_RXADDR;  
    w_register(REG_EN_RXADDR,&val,sizeof(val));
    
    val = RST_SETUP_AW;   
    w_register(REG_SETUP_AW,&val,sizeof(val));
    
    val = RST_SETUP_RETR; 
    w_register(REG_SETUP_RETR,&val,sizeof(val));
    
    val = RST_RF_CH ;     
    w_register(REG_RF_CH,&val,sizeof(val));
    
    val = RST_RF_SETUP ;  
    w_register(REG_RF_SETUP,&val,sizeof(val));
    
    val = RST_STATUS  ;   /* Clear interrupt pending bits */
    w_register(REG_STATUS,&val,sizeof(val));

    
    val = RST_OBSERVE_TX ; /* skip */
    
    val = RST_RPD ;  /* skip */
    
    val5[0] = RST_RX_ADDR_P0 ;
    val5[1] = RST_RX_ADDR_P0 ;
    val5[2] = RST_RX_ADDR_P0 ;
    val5[3] = RST_RX_ADDR_P0 ;
    val5[4] = RST_RX_ADDR_P0 ;
    w_register(REG_RX_ADDR_P0,val5,sizeof(val5));
    
    val5[0] = RST_RX_ADDR_P1 ;
    val5[1] = RST_RX_ADDR_P1 ;
    val5[2] = RST_RX_ADDR_P1 ;
    val5[3] = RST_RX_ADDR_P1 ;
    val5[4] = RST_RX_ADDR_P1 ;
    w_register(REG_RX_ADDR_P1,val5,sizeof(val5));
    
    val = RST_RX_ADDR_P2 ;
    w_register(REG_RX_ADDR_P2,&val,sizeof(val));
    
    val = RST_RX_ADDR_P3 ;
    w_register(REG_RX_ADDR_P3,&val,sizeof(val));
    
    val = RST_RX_ADDR_P4 ;
    w_register(REG_RX_ADDR_P4,&val,sizeof(val));
    
    val = RST_RX_ADDR_P5 ;
    w_register(REG_RX_ADDR_P5,&val,sizeof(val));
    
    val5[0] = RST_TX_ADDR ; 
    val5[1] = RST_TX_ADDR ; 
    val5[2] = RST_TX_ADDR ; 
    val5[3] = RST_TX_ADDR ; 
    val5[4] = RST_TX_ADDR ; 
    w_register(REG_TX_ADDR,val5,sizeof(val5));

    
    val = RST_RX_PW_P0 ;  
    w_register(REG_RX_PW_P0,&val,sizeof(val));
    
    val = RST_RX_PW_P1 ;  
    w_register(REG_RX_PW_P1,&val,sizeof(val));
    
    val = RST_RX_PW_P2 ;  
    w_register(REG_RX_PW_P2,&val,sizeof(val));
    
    val = RST_RX_PW_P3 ;  
    w_register(REG_RX_PW_P3,&val,sizeof(val));

    val = RST_RX_PW_P4 ;  
    w_register(REG_RX_PW_P4,&val,sizeof(val));
    
    val = RST_RX_PW_P5 ;  
    w_register(REG_RX_PW_P5,&val,sizeof(val));
    
    val = RST_FIFO_STATUS; /* skip */
    
    val = RST_DYNPD;     
    w_register(REG_DYNPD,&val,sizeof(val));
    
    val = RST_FEATURE ;   
    w_register(REG_FEATURE,&val,sizeof(val));

    
    /* 
         * Configure registers as ShockBurst Setting
         */
    /* Set CONFIG = 0x78 : Disable IRQ PIN, Enable CRC with 1Byte, Power Down,PTX */
    val = 0x78;
    w_register(REG_CONFIG,&val,sizeof(val));
    
    /* Set EN_AA = 0x00: Disable all auto acknowledgement data pipes */
    val = 0x00;
    w_register(REG_EN_AA,&val,sizeof(val));

    /* Set EN_RXADDR = 0x01: Only enable RX address pipe 0  */
    val = 0x01;
    w_register(REG_EN_RXADDR,&val,sizeof(val));

    /* Set SETUP_RETR = 0x00: Disable retransmit feature */
    val = 0x00;
    w_register(REG_SETUP_RETR,&val,sizeof(val));

    /* Maybe we need Receved Power Detector */
    
    /* Set RF_CH = rf_ch */
    if( rf_ch < 2 || rf_ch > 123) rf_ch = RST_RF_CH;
    val = rf_ch;
    w_register(REG_RF_CH,&val,sizeof(val));

    /* Set RF_SETUP =  */
    val = 0;
    switch(rate) {
        case AIR_RATE_2Mbps: 
            val |= 0x08;
            break;
        case AIR_RATE_1Mbps: 
            val |= 0x00;
            break;
        case AIR_RATE_256Kbps: 
            val |= 0x20;
            break;
        default: /* Using 2Mbps */
            val |= 0x08;
            break;
    }
    switch(pwr) {
        case PWR_0dBm:
            val |= 0x03<<1;
            break;
        case PWR_n6dBm:
            val |= 0x02<<1;
            break;   
        case PWR_n12dBm:
            val |= 0x01<<1;
            break;
        case PWR_n18dBm:
            val |= 0x00<<1;
            break;
        default: /* Using 0dBm */
            val |= 0x03<<1;
            break;
    }
    w_register(REG_RF_SETUP,&val,sizeof(val));

    /* set rx address ,always to rx data pipe 0 */
    set_rx_addr(rx_addr);

    /* set payload width , always to rx data pipe 0*/
    set_payload_width(PAYLOAD_MAX_WIDTH);
    
    /* Set DYNPD = 0x00: Disable all dynamic payload length data pipes. */
    val = 0x00;
    w_register(REG_DYNPD,&val,sizeof(val));

    /* Set FETURE = 0x00: Disable all fetures */
    val = 0x00;
    w_register(REG_FEATURE,&val,sizeof(val));

    /* Here registers setting done */

    
    /* POWER UP and enter Stdby1 mode 
         */
    set_to_POWER_UP();
    
    
    /* Flush TX_FIFO and RX_FIFO */
    set_to_PRX();
    flush_rx();
    set_to_PTX();
    flush_tx();
    
    /* Clear all interrupt pending bits */
    clear_interrupt_all();
    
}

void NRF24L01P::end(void)
{
    set_ce_low();
    delay(20);
    set_to_POWER_DOWN();
}




/*
* @name    : send_packet
* @desc      : send buffer data to dest
* @param   : dest - destination address (MUST be 5byte)
*                    buffer - data buffer to send (MUST be 24byte)
*                    
* @return   : 0 - success 
*                    1 - RX ACK_PNO packet timeout.
*                    2 - ACK_PNO width != 8
*                    3 - ack_src != dest
*                    4 - PNO err
*/
int8_t NRF24L01P::send_packet(uint8_t *dest, uint8_t *buffer)
{
    static uint8_t PNO=0;
    
    uint8_t i;    
    uint8_t status = 0;
    uint32_t cnt = 0;
    uint8_t width;
    
    boolean tx_ds = false;
    boolean rx_dr = false;
    
    /* 1. ce = 0,enter PTX flush TX_FIFO */
    set_ce_low();
    set_to_PTX();
    flush_tx();
    
    /* 2. clear all interrupt pending bits */
    clear_interrupt_all();

    /* 3. set tx address with dest, */
    set_tx_addr(dest);

    /* 4.  PNO++, pack packet(src,PNO,Reserved,user data), clock TX_PAYLOAD into nRF24L01+ through the SPI */
    PNO ++;
    for(i=0;i<TXRX_ADDR_WIDTH;i++)
        tx_buffer[i]=rx_addr[i];    /* src:Byte0-Byte4 */
    tx_buffer[i++] = PNO;           /* PNO:Byte5 */
    tx_buffer[i++] = 0;             /* Reserved:Byte6 */
    tx_buffer[i++] = 0;             /* Reserved:Byte7 */
    for(;i<PAYLOAD_MAX_WIDTH;i++)   /* User Data:Byte8-Byte31 */
        tx_buffer[i]=buffer[i-8];
    w_tx_payload(tx_buffer,sizeof(tx_buffer));

    /* 5. a high pluse on ce pin starts the transmission */
    set_ce_high();
    delayMicroseconds(15);
    set_ce_low();

    /* 6. waitting for interrupt coming (TX_DS), delay 1 us, auto enter STDBY1 (now,ce should be low) */    
    while(1) {
        status = r_status();
        tx_ds =  (status & (0x01<<5)) ? true : false;
        if(tx_ds) {
            delayMicroseconds(1);    
            break;
        }
        delayMicroseconds(5);
    }

    /* 7. Immeidatly ce = 0 , enter PRX, flush RX_FIFO,  */
    set_to_PRX();
    flush_rx();

    /* 8. clear all interrupt pending bits */
    clear_interrupt_all();

    /* 9. set payload width=8bytes (|ack_src:5B|PNO:1B|Reserved:2B|) */
    set_payload_width(8);

    /* 10. ce = 1,after 130us, to receive ACK_PNO packet. */
    set_ce_high();
    delayMicroseconds(130);

    /* 11. waitting for interrupt coming RX_DR ( or Timeout  ce = 0, return 1;) */
    cnt = 0;
    while(1) {
        status = r_status();
        rx_dr =  (status & (0x01<<6)) ? true : false;
        if(rx_dr) {
            break;
        }
        delayMicroseconds(10);
        cnt += 10;
        if(cnt >= 50000) { /* time out > 50 ms */
            set_ce_low();
            return 1;
        }
    }
    
    /* 12. ce = 0,auto enter STDBY1 mode */
    set_ce_low();

    /* 13. read ACK_PNO and compire */
    r_rx_pl_wid(&width);
    
    if(width != 8)
        return 2;
    
    r_rx_payload(rx_buffer,width);

    for(i=0;i<5;i++) {
        if(rx_buffer[i] != tx_addr[i])
            return 3;
    }
    if( rx_buffer[i] != PNO)
        return 4;
    
    return 0;
}


/*
* @name    : recv_packet
* @desc      : recv data
* @param   : src - from where (MUST be 5byte)
*                    buffer - data buffer received (MUST be24byte)
* @return   : 0 - success
*                    1 - RX PLD length < 32
*/
int8_t NRF24L01P::recv_packet(uint8_t *src, uint8_t *buffer)
{
    uint8_t status = 0;
    uint8_t i;
    uint32_t cnt = 0;
    int8_t ret = 0;
    uint8_t width;
    
    boolean tx_ds = false;
    boolean rx_dr = false;
    boolean max_rt = false;
    
    /* 1. ce = 0,enter PRX flush RX_FIFO */
    set_ce_low();
    set_to_PRX();
    flush_rx();

    /* 2. clear all interrupt pending bits */
    clear_interrupt_all();

    /* 3. set payload width=32(|src:5B|PNO:1B|Reserved:2B|User Data:24B|) */
    set_payload_width(PAYLOAD_MAX_WIDTH);

    /* 4. ce = 1,after 130us, to receive data packet. */
    set_ce_high();
    delayMicroseconds(130);
    
    /* 5. waitting for interrupt coming RX_DR */
    while(1) {
        status = r_status();
        rx_dr =  (status & (0x01<<6)) ? true : false;
        if( rx_dr )
            break;
        delayMicroseconds(7);
    }

    /* 6. ce = 0,auto enter STDBY1 mode */
    set_ce_low();

    /* 7. read RX_FIFO: src -> src, User Data -> buffer, */
    r_rx_pl_wid(&width);
    if(width != PAYLOAD_MAX_WIDTH)
        return 1;
    
    r_rx_payload(rx_buffer,width);
    for(i=0;i<TXRX_ADDR_WIDTH;i++)
        src[i] = rx_buffer[i];
    for(i=0;i<USER_DATA_WIDTH;i++)
        buffer[i] = rx_buffer[i+8];

    /* 8. set tx address with src */
    set_tx_addr(src);

    /* 9. enter PTX flush TX_FIFO */
    set_to_PTX();
    flush_tx();

    /* 10. clear all interrupt pending bits */
    clear_interrupt_all();

    /* 11. pack packet(ack_src,PNO), clock TX_PAYLOAD into nRF24L01+ through the SPI */
    for(i=0;i<TXRX_ADDR_WIDTH;i++)
        tx_buffer[i]=rx_addr[i];
    tx_buffer[i++] = rx_buffer[5];
    tx_buffer[i++] = 0;
    tx_buffer[i++] = 0;

    w_tx_payload(tx_buffer,8);

    /* Delay 250us */
    delayMicroseconds(250);
    
    /* 12. a high pluse on ce pin starts the transmission */
    set_ce_high();
    delayMicroseconds(15);
    set_ce_low();

    /* 13. waitting for interrupt coming (TX_DS), delay 2 us, auto enter STDBY1 (now,ce should be low) */
    while(1) {
        status = r_status();
        tx_ds =  (status & (0x01<<5)) ? true : false;
        if( tx_ds )
            break;
        delayMicroseconds(5);
    }

    return 0;
    
}


void NRF24L01P::set_payload_width(uint8_t width)
{
    w_register(REG_RX_PW_P0,&width,sizeof(width));
}


void NRF24L01P::set_tx_addr(uint8_t *tx_addr)
{
    uint8_t i;
    for(i=0;i<TXRX_ADDR_WIDTH;i++){
        if(this->tx_addr[i] != tx_addr[i])
            break;
    }
    if( i<TXRX_ADDR_WIDTH ) {
        for(i=0;i<TXRX_ADDR_WIDTH;i++)
            this->tx_addr[i] = tx_addr[i];
        w_register(REG_TX_ADDR,tx_addr,TXRX_ADDR_WIDTH);
    }
    
}

void NRF24L01P::set_rx_addr(uint8_t *rx_addr)
{
    uint8_t i;
    for(i=0;i<TXRX_ADDR_WIDTH;i++)
        this->rx_addr[i] = rx_addr[i];
    /* Using RX pipe 0 only */
    w_register(REG_RX_ADDR_P0,rx_addr,TXRX_ADDR_WIDTH);
}


void NRF24L01P::clear_interrupt_RX_DR(void)
{
    uint8_t val;
   
    val = (0x01<<6);
    w_register(REG_STATUS,&val,sizeof(val));
}
void NRF24L01P::clear_interrupt_TX_DS(void)
{
    uint8_t val;
   
    val = (0x01<<5);
    w_register(REG_STATUS,&val,sizeof(val));
}

void NRF24L01P::clear_interrupt_MAX_RT(void)
{
    uint8_t val;
    
    val = (0x01<<4);
    w_register(REG_STATUS,&val,sizeof(val));
}

void NRF24L01P::clear_interrupt_all(void)
{
    uint8_t val;
    
    val = (0x07<<4);
    w_register(REG_STATUS,&val,sizeof(val));
}


void NRF24L01P::set_to_POWER_UP(void)
{
    uint8_t val;
    r_register(REG_CONFIG,&val,sizeof(val));
    val |= 0x01<<1;
    w_register(REG_CONFIG,&val,sizeof(val));
    delay(10);
}
void NRF24L01P::set_to_POWER_DOWN(void)
{
    uint8_t val;
    r_register(REG_CONFIG,&val,sizeof(val));
    val &= ~(0x01<<1);
    w_register(REG_CONFIG,&val,sizeof(val));
    delay(1);
}

void NRF24L01P::set_to_PTX(void)
{
    uint8_t val;
    r_register(REG_CONFIG,&val,sizeof(val));
    val &= 0xFE;
    w_register(REG_CONFIG,&val,sizeof(val));
}
void NRF24L01P::set_to_PRX(void)
{
    uint8_t val;
    r_register(REG_CONFIG,&val,sizeof(val));
    val |= 0x01;
    w_register(REG_CONFIG,&val,sizeof(val));
}




/* ***************************************************************************
* nRF24L01P Command Functions
* ***************************************************************************/

uint8_t NRF24L01P::w_register(uint8_t addr,uint8_t *buffer,uint8_t len)
{
    uint8_t status;
    uint8_t i;
    
    spi_cs_low();
    status = spi_transfer(CMD_W_REGISTER(addr));
    for(i=0;i<len;i++) {
        spi_transfer(buffer[i]);
    }
    spi_cs_high();
    return status;
}

uint8_t NRF24L01P::r_register(uint8_t addr,uint8_t *buffer,uint8_t len)
{
    uint8_t status;
    uint8_t i;
    
    spi_cs_low();
    status = spi_transfer(CMD_R_REGISTER(addr));
    for(i=0;i<len;i++) {
        buffer[i] = spi_transfer(0);
    }
    spi_cs_high();
    return status;
}

uint8_t NRF24L01P::r_rx_payload(uint8_t *buffer,uint8_t len)
{
    uint8_t status;
    uint8_t i;
    
    spi_cs_low();
    status = spi_transfer(CMD_R_RX_PAYLOAD);
    for(i=0;i<len;i++) {
        buffer[i] = spi_transfer(0);
    }
    spi_cs_high();
    return status;
}
uint8_t NRF24L01P::w_tx_payload(uint8_t *buffer,uint8_t len)
{
    uint8_t status;
    uint8_t i;
    
    spi_cs_low();
    status = spi_transfer(CMD_W_TX_PAYLOAD);
    for(i=0;i<len;i++) {
        spi_transfer(buffer[i]);
    }
    spi_cs_high();
    return status;
}

uint8_t NRF24L01P::flush_tx(void)
{
    uint8_t status;
    spi_cs_low();
    status = spi_transfer(CMD_FLUSH_TX);
    spi_cs_high();
    return status;
}
uint8_t NRF24L01P::flush_rx(void)
{
    uint8_t status;
    spi_cs_low();
    status = spi_transfer(CMD_FLUSH_RX);
    spi_cs_high();
    return status;
}
uint8_t NRF24L01P::reuse_tx_pl(void)
{
    uint8_t status;
    spi_cs_low();
    status = spi_transfer(CMD_REUSE_TX_PL);
    spi_cs_high();
    return status;
}
uint8_t NRF24L01P::r_rx_pl_wid(uint8_t *width)
{
    uint8_t status;
    spi_cs_low();
    status = spi_transfer(CMD_R_RX_PL_WID);
    *width = spi_transfer(0);
    spi_cs_high();
    return status;
}

uint8_t NRF24L01P::w_ack_payload(uint8_t ppp,uint8_t *buffer,uint8_t len)
{
    uint8_t status;
    uint8_t i;
    
    spi_cs_low();
    status = spi_transfer(CMD_W_ACK_PAYLOAD(ppp));
    for(i=0;i<len;i++) {
        spi_transfer(buffer[i]);
    }
    spi_cs_high();
    return status;    
}
uint8_t NRF24L01P::w_tx_payload_noack(uint8_t *buffer,uint8_t len)
{
    uint8_t status;
    uint8_t i;
    
    spi_cs_low();
    status = spi_transfer(CMD_W_TX_PAYLOAD_NOACK);
    for(i=0;i<len;i++) {
        spi_transfer(buffer[i]);
    }
    spi_cs_high();
    return status;      
}
uint8_t NRF24L01P::r_status(void)
{
    uint8_t status;
    spi_cs_low();
    status = spi_transfer(CMD_R_STATUS);
    spi_cs_high();
    return status;
}


/* ***************************************************************************
* Hardware Functions
* ***************************************************************************/

void NRF24L01P::set_ce_high(void)
{
    digitalWrite(ce,HIGH);
    delayMicroseconds(6); /* Tpece2csn : Delay from CE positive edge to CSN low 4 us */
}
void NRF24L01P::set_ce_low(void)
{
    digitalWrite(ce,LOW);
}
void NRF24L01P::spi_cs_high(void)
{
    digitalWrite(spi_cs,HIGH);
}
void NRF24L01P::spi_cs_low(void)
{
    digitalWrite(spi_cs,LOW);
}

uint8_t NRF24L01P::spi_transfer(uint8_t data)
{
    return _SPI->transfer(data);
}

