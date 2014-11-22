// PN532 library by adafruit/ladyada
// MIT license

// authenticateBlock, readMemoryBlock, writeMemoryBlock contributed
// by Seeed Technology Inc (www.seeedstudio.com)

// PN532 library was Transplanted by Wu Pengfei to compitable with 
// Segnix <https://github.com/iteadsw/Segnix>.
// Company:ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
// Date: 2014/05/22
// Email: pengfei.wu@itead.cc


#ifndef __PN532_H__
#define __PN532_H__

#if 0 // disabled by wpf
#include <Arduino.h>
#include <SPI.h>
#else
#include <itead.h>
#endif

//#define PN532DEBUG 1

/*If define PN532_P2P_DEBUG, all the data that initiator and target received */
/*in the peer to peer communication, will be printed in the serial port tool window*/
//#define PN532_P2P_DEBUG 1

#define PN532_PREAMBLE 0x00
#define PN532_STARTCODE1 0x00
#define PN532_STARTCODE2 0xFF
#define PN532_POSTAMBLE 0x00

#define PN532_HOSTTOPN532 0xD4

#define PN532_FIRMWAREVERSION 0x02
#define PN532_GETGENERALSTATUS 0x04
#define PN532_SAMCONFIGURATION  0x14
#define PN532_INLISTPASSIVETARGET 0x4A
#define PN532_INDATAEXCHANGE 0x40
#define PN532_INJUMPFORDEP 0x56
#define PN532_TGINITASTARGET 0x8C
#define PN532_TGGETDATA 0x86
#define PN532_TGSETDATA 0x8E

#define PN532_MIFARE_READ 0x30
#define PN532_MIFARE_WRITE 0xA0

#define PN532_AUTH_WITH_KEYA 0x60
#define PN532_AUTH_WITH_KEYB 0x61


#define PN532_WAKEUP 0x55

#define  PN532_SPI_STATREAD  0x02
#define  PN532_SPI_DATAWRITE 0x01
#define  PN532_SPI_DATAREAD  0x03

#define  PN532_SPI_READY  0x01

#define PN532_MIFARE_ISO14443A 0x0

#define KEY_A	1
#define KEY_B	2

#define PN532_BAUDRATE_201K 1
#define PN532_BAUDRATE_424K 2

class PN532{
public:
  PN532(uint8_t cs,SPIClass &SPI);
  void begin(void);			
  boolean SAMConfig(void);
  uint32_t getFirmwareVersion(void);
  uint32_t readPassiveTargetID(uint8_t cardbaudrate);
  uint32_t authenticateBlock(
        uint8_t cardnumber /*1 or 2*/,
        uint32_t cid /*Card NUID*/,
        uint8_t blockaddress /*0 to 63*/,
        uint8_t authtype /*Either KEY_A or KEY_B */,
        uint8_t* keys
  );
  boolean readMemoryBlock(
        uint8_t cardnumber /*1 or 2*/,
        uint8_t blockaddress /*0 to 63*/, 
        uint8_t * block
  );
  boolean writeMemoryBlock(
        uint8_t cardnumber /*1 or 2*/,
        uint8_t blockaddress /*0 to 63*/, 
        uint8_t * block
  );

  uint32_t configurePeerAsInitiator(uint8_t baudrate /* Any number from 0-2. 0 for 106kbps or 1 for 201kbps or 2 for 424kbps */); //106kps is not supported
  uint32_t configurePeerAsTarget(); 
  boolean initiatorTxRx(char* dataOut,char* dataIn);
  uint32_t targetTxRx(char* dataOut,char* dataIn);

  boolean sendCommandCheckAck(
        uint8_t *cmd, 
        uint8_t cmdlen, 
        uint16_t timeout = 1000
  );

private:
  uint8_t _cs;//Chip seclect pin for PN532 is optional.
  #if 0 // disabled by wpf
  SPIClass pn532_SPI;
  #else
  SPIClass *pn532_SPI;
  #endif
  void write(uint8_t _data);
  uint8_t read(void);
  uint8_t readSpiStatus(void);
  boolean checkSpiAck();
  void read(uint8_t* buff, uint8_t n);
  void writeCommand(uint8_t* cmd, uint8_t cmdlen);
};

#endif

