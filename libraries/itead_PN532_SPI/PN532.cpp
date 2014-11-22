// PN532 library by adafruit/ladyada
// MIT license

/* authenticateBlock, readMemoryBlock, writeMemoryBlock contributed  */
/* by Seeed Technology Inc (www.seeedstudio.com)										 */
/* Modified history:                                                 */
/* Nov 5,2012 modified by Frankie.Chu at Seeed.                      */
/*   Add some program comments of some key functions;                */
/*	 Modified the basic interface functions spiwrite() and spiread() */
/*     to be write() and read(),and they will call the functions of  */
/*     class SPIClass but not to simulate timing of SPI;             */
/*	 Rename all of the functions to be like the abAbAb;              */
/*   Modified the constructor of PN532 class;                        */

// PN532 library was Transplanted by Wu Pengfei to compitable with 
// Segnix <https://github.com/iteadsw/Segnix>.
// Company:ITEAD Intelligent Systems Co.,Ltd.(http://imall.iteadstudio.com/)
// Date: 2014/05/22
// Email: pengfei.wu@itead.cc


#if 0 // disabled by wpf
#include <Arduino.h>
#include <SPI.h>
#else
#include <itead.h>
#endif
#include "PN532.h"


byte pn532ack[] = {
  0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
byte pn532response_firmwarevers[] = {
  0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03};

#define PN532_PACK_BUFF_SIZE 64
byte pn532_packetbuffer[PN532_PACK_BUFF_SIZE];

static byte BitReverseTable256[] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,                      
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,                      
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,                      
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,                      
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,                      
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,                      
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,                      
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,                      
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,                      
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,                      
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,                      
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,                      
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,                      
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,                      
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,                      
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};


/*Construct PN532 object and construct PN532's own SPI interface object */
/* at the same time.*/
PN532::PN532(uint8_t cs,SPIClass &SPI):pn532_SPI(&SPI),_cs(cs)
{
  pinMode(_cs,OUTPUT);
  digitalWrite(_cs,HIGH);
}

void PN532::begin() 
{
  pn532_SPI->begin();
  /*The mode of the SPI interface should be set at mode0 
   	  according to the datasheet of PN532 */
  pn532_SPI->setDataMode(SPI_MODE0);
  pn532_SPI->setBitOrder(LSBFIRST);
  /*Set the SPI frequency to be one sixteenth of the 
   	  frequency of the system clock*/
  pn532_SPI->setClockDivider(SPI_CLOCK_DIV128);
  digitalWrite(_cs, LOW);
  delay(1000); 
  /*Not exactly sure why but we have to send a dummy command to get synced up*/
  pn532_packetbuffer[0] = PN532_FIRMWAREVERSION;
  /*Ignore response!*/
  if (sendCommandCheckAck(pn532_packetbuffer, 1)) {
  } else {
    printf("init err\n");
  }

}

uint32_t PN532::getFirmwareVersion(void) 
{
  uint32_t response;

  pn532_packetbuffer[0] = PN532_FIRMWAREVERSION;

  if (!sendCommandCheckAck(pn532_packetbuffer, 1))
    return 0;

  //Read data packet
  read(pn532_packetbuffer, 12);
  //Check some basic stuff
  if (0 != strncmp((char *)pn532_packetbuffer, (char *)pn532response_firmwarevers, 6)) 
  {
    return 0;
  }

  response = pn532_packetbuffer[6];
  response <<= 8;
  response |= pn532_packetbuffer[7];
  response <<= 8;
  response |= pn532_packetbuffer[8];
  response <<= 8;
  response |= pn532_packetbuffer[9];

  return response;
}

/**********************************************************************/
/*Function: Send command to PN532 with SPI and check the ACK.          */
/*Parameter:-uint8_t* cmd,The pointer that saves the command code to be sent;*/
/*          -uint8_t cmd_len,The number of bytes of the command;        */
/*	    -uint16_t timeout,default timeout is one second            */
/*Return:   boolean,ture = send command successfully	               */
boolean PN532::sendCommandCheckAck(uint8_t* cmd, uint8_t cmd_len, uint16_t timeout) 
{
  uint16_t timer = 0;

  // write the command
  writeCommand(cmd, cmd_len);

  // Wait for chip to say it's ready!
  while (readSpiStatus() != PN532_SPI_READY) {
    if (timeout != 0) {
      timer+=10;
      if (timer > timeout)
        return false;
    }
    delay(10);
  }

  // read acknowledgement
  if (!checkSpiAck()) {
    return false;
  }

  timer = 0;
  // Wait for chip to say its ready!
  while (readSpiStatus() != PN532_SPI_READY) {
    if (timeout != 0) {
      timer+=10;
      if (timer > timeout)
        return false;
    }
    delay(10);
  }

  return true; // ack'd command
}

boolean PN532::SAMConfig(void) 
{
  pn532_packetbuffer[0] = PN532_SAMCONFIGURATION;
  pn532_packetbuffer[1] = 0x01; // normal mode;
  pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
  pn532_packetbuffer[3] = 0x01; // use IRQ pin!

  if (! sendCommandCheckAck(pn532_packetbuffer, 4))
    return false;

  // read data packet
  read(pn532_packetbuffer, 8);

  return  (pn532_packetbuffer[5] == 0x15);
}

/**********************************************************************/
/*Function: Configure the NFC shield as initiator in the peer to peer */
/*	      commnunication and only the initiator set the baud rate.*/
/*Parameter:-uint8_t baudrate,Any number from 0-2. 0 for 106kbps or 
 	    1 for 201kbps or 2 for 424kbps but 106kps is not supported yet;*/
/*Return: boolean,ture = the shield finds the target and is configured */
/*	as initiator successfully.                      	       */
uint32_t PN532::configurePeerAsInitiator(uint8_t baudrate) 
{

  pn532_packetbuffer[0] = PN532_INJUMPFORDEP;
  pn532_packetbuffer[1] = 0x01; //Active Mode
  pn532_packetbuffer[2] = baudrate;// Use 1 or 2. //0 i.e 106kps is not supported yet
  pn532_packetbuffer[3] = 0x01; //Indicates Optional Payload is present

  //Polling request payload
  pn532_packetbuffer[4] = 0x00; 
  pn532_packetbuffer[5] = 0xFF; 
  pn532_packetbuffer[6] = 0xFF; 
  pn532_packetbuffer[7] = 0x00; 
  pn532_packetbuffer[8] = 0x00; 

  if (!sendCommandCheckAck(pn532_packetbuffer, 9))
    return false;

  // read data packet
  read(pn532_packetbuffer, 19+6);

#ifdef PN532DEBUG
  printf("\n");
  // check the response
  printf("PEER_INITIATOR\n");
  for(uint8_t i=0;i<19+6;i++)
  {
    printf("%X ",pn532_packetbuffer[i]); 
  }
#endif

  return (pn532_packetbuffer[7] == 0x00); //No error

}
/**********************************************************************/
/*Function: Transmit to the target and receive from the target.       */
/*Parameter:-char* dataOut,data buffer to send;                       */
/*	    -char* dataIn,data buffer to save the data receive.       */
/*Return:   boolean,ture = No error                                   */
boolean PN532::initiatorTxRx(char* dataOut,char* dataIn)
{
  pn532_packetbuffer[0] = PN532_INDATAEXCHANGE;
  pn532_packetbuffer[1] = 0x01; //Target 01

    for(uint8_t iter=(2+0);iter<(2+16);iter++)
  {
    pn532_packetbuffer[iter] = dataOut[iter-2]; //pack the data to send to target
  }

  if (! sendCommandCheckAck(pn532_packetbuffer, 18))
    return false;

  // read data packet
  read(pn532_packetbuffer, 18+6);

#ifdef PN532_P2P_DEBUG
  // check the response
  printf("\nINITIATOR receive:\n");
  for(uint8_t i=0;i<18+6;i++)
  {
    printf("%X ",pn532_packetbuffer[i]); 
  }
#endif

  for(uint8_t iter=8;iter<(8+16);iter++)
  {
    dataIn[iter-8] = pn532_packetbuffer[iter]; //data received from target
  }

  return (pn532_packetbuffer[7] == 0x00); //No error
}

uint32_t PN532::configurePeerAsTarget() 
{
  byte pbuffer[38] =      { 
    PN532_TGINITASTARGET, 
    0x00,
    0x08, 0x00, //SENS_RES
    0x12, 0x34, 0x56, //NFCID1
    0x40, //SEL_RES
    0x01, 0xFE, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, // POL_RES
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 
    0xFF, 0xFF,
    0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, //NFCID3t: Change this to desired value
    0x00, 0x00 //Length of general and historical bytes
  };
  for(uint8_t i = 0;i < 38;i ++)
  {
    pn532_packetbuffer[i] = pbuffer[i];
  }
  if (! sendCommandCheckAck(pn532_packetbuffer, 38))
    return false;

  // read data packet
  read(pn532_packetbuffer, 18+6);

#ifdef PN532DEBUG
  // check some basic stuff

  printf("\nPEER_TARGET\n");
  for(uint8_t i=0;i<18+6;i++)
  {
    printf("%X ",pn532_packetbuffer[i]); 
  }
#endif

  return (pn532_packetbuffer[23] == 0x00); //No error as it received all response
}
/*Function: Recieve data first and then transmit data to the initiator*/
uint32_t PN532::targetTxRx(char* dataOut,char* dataIn)
{
  /* Receiving from Initiator */
  pn532_packetbuffer[0] = PN532_TGGETDATA;
  if (! sendCommandCheckAck(pn532_packetbuffer, 1))
    return false;

  // read data packet
  read(pn532_packetbuffer, 18+6);

#ifdef PN532_P2P_DEBUG
  // check the response
  printf("\nTARGET RX:\n");
  for(uint8_t i=0;i<18+6;i++)
  {
    printf("%X ",pn532_packetbuffer[i]); 
  }
#endif

  for(uint8_t iter=8;iter<(8+16);iter++)
  {
    dataIn[iter-8] = pn532_packetbuffer[iter]; //data received from initiator
  }

  /* Sending to Initiator */
  if(pn532_packetbuffer[7] == 0x00) //If no errors in receiving, send data.
  {
    pn532_packetbuffer[0] = PN532_TGSETDATA;
    for(uint8_t iter=(1+0);iter<(1+16);iter++)
    {
      pn532_packetbuffer[iter] = dataOut[iter-1]; //pack the data to send to target
    }

    if (! sendCommandCheckAck(pn532_packetbuffer, 17))
      return false;

    // read data packet
    read(pn532_packetbuffer, 2+6);

#ifdef PN532_P2P_DEBUG
    // check the response
    println("\nTARGET get response after transmiting: \n");
    for(uint8_t i=0;i<2+6;i++)
    {
      printf("%X ",pn532_packetbuffer[i]); 
    }
#endif

    return (pn532_packetbuffer[7] == 0x00); //No error
  }

}

uint32_t PN532::authenticateBlock(uint8_t cardnumber /*1 or 2*/,uint32_t cid /*Card NUID*/, uint8_t blockaddress /*0 to 63*/,uint8_t authtype/*Either KEY_A or KEY_B */, uint8_t * keys) 
{
  pn532_packetbuffer[0] = PN532_INDATAEXCHANGE;
  pn532_packetbuffer[1] = cardnumber;  // either card 1 or 2 (tested for card 1)
  if(authtype == KEY_A)
  {
    pn532_packetbuffer[2] = PN532_AUTH_WITH_KEYA;
  }
  else
  {
    pn532_packetbuffer[2] = PN532_AUTH_WITH_KEYB;
  }
  pn532_packetbuffer[3] = blockaddress; //This address can be 0-63 for MIFARE 1K card

  pn532_packetbuffer[4] = keys[0];
  pn532_packetbuffer[5] = keys[1];
  pn532_packetbuffer[6] = keys[2];
  pn532_packetbuffer[7] = keys[3];
  pn532_packetbuffer[8] = keys[4];
  pn532_packetbuffer[9] = keys[5];

  pn532_packetbuffer[10] = ((cid >> 24) & 0xFF);
  pn532_packetbuffer[11] = ((cid >> 16) & 0xFF);
  pn532_packetbuffer[12] = ((cid >> 8) & 0xFF);
  pn532_packetbuffer[13] = ((cid >> 0) & 0xFF);

  if (! sendCommandCheckAck(pn532_packetbuffer, 14))
    return false;

  // read data packet
  read(pn532_packetbuffer, 2+6);

#ifdef PN532DEBUG
  for(int iter=0;iter<14;iter++)
  {
    printf("%X ",pn532_packetbuffer[iter]);
  }
  // check some basic stuff

  printf("\nAUTH\n");
  for(uint8_t i=0;i<2+6;i++)
  {
    printf("%X \n",pn532_packetbuffer[i]); 
  }
#endif

  if((pn532_packetbuffer[6] == 0x41) && (pn532_packetbuffer[7] == 0x00))
  {
    return true;
  }
  else
  {
    return false;
  }

}
/****************************************************************************/
/*Function: Read a block(16 bytes) from the tag and stores in the parameter.*/
/*Parameter:-uint8_t cardnumber,can be 1 or 2;                              */
/*          -blockaddress,range from 0 to 63;                               */
/*	    -uint8_t* block,will save 16bytes that read from tag.           */
/*Return:   boolean         					      	    */
boolean PN532::readMemoryBlock(uint8_t cardnumber,uint8_t blockaddress,uint8_t * block)
{
  pn532_packetbuffer[0] = PN532_INDATAEXCHANGE;
  pn532_packetbuffer[1] = cardnumber;  // either card 1 or 2 (tested for card 1)
  pn532_packetbuffer[2] = PN532_MIFARE_READ;
  pn532_packetbuffer[3] = blockaddress; //This address can be 0-63 for MIFARE 1K card

  if (! sendCommandCheckAck(pn532_packetbuffer, 4))
    return false;

  // read data packet
  read(pn532_packetbuffer, 18+6);
  // check some basic stuff
#ifdef PN532DEBUG
  printf("READ\n");
#endif
  for(uint8_t i=8;i<18+6;i++)
  {
    block[i-8] = pn532_packetbuffer[i];
#ifdef PN532DEBUG
    printf("%X ",pn532_packetbuffer[i]); 
#endif
  }
  if((pn532_packetbuffer[6] == 0x41) && (pn532_packetbuffer[7] == 0x00))
  {
    return true; //read successful
  }
  else

  {
    return false;
  }

}


/****************************************************************************/
/*Function: Write a block(16 bytes) to the tag.                             */
/*Parameter:-uint8_t cardnumber,can be 1 or 2;                              */
/*          -blockaddress,range from 0 to 63;                               */
/*	    -uint8_t* block,saves 16bytes that will write to the tag.       */
/*Return:  boolean							    */
/*Note:Donot write to Sector Trailer Block unless you know what you are doing.*/
boolean PN532::writeMemoryBlock(uint8_t cardnumber,uint8_t blockaddress,uint8_t * block) 
{
  pn532_packetbuffer[0] = PN532_INDATAEXCHANGE;
  pn532_packetbuffer[1] = cardnumber;  // either card 1 or 2 (tested for card 1)
  pn532_packetbuffer[2] = PN532_MIFARE_WRITE;
  pn532_packetbuffer[3] = blockaddress;

  for(uint8_t byte=0; byte <16; byte++)
  {
    pn532_packetbuffer[4+byte] = block[byte];
  }

  if (! sendCommandCheckAck(pn532_packetbuffer, 20))
    return false;
  // read data packet
  read(pn532_packetbuffer, 2+6);

#ifdef PN532DEBUG
  // check some basic stuff
  printf("WRITE\n");
  for(uint8_t i=0;i<2+6;i++)
  {
    printf("%X ",pn532_packetbuffer[i]); 
  }
#endif

  if((pn532_packetbuffer[6] == 0x41) && (pn532_packetbuffer[7] == 0x00))
  {
    return true; //write successful
  }
  else
  {
    return false;
  }
}

uint32_t PN532::readPassiveTargetID(uint8_t cardbaudrate) 
{
  uint32_t cid;

  pn532_packetbuffer[0] = PN532_INLISTPASSIVETARGET;
  pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this to 2 later)
  pn532_packetbuffer[2] = cardbaudrate;

  if (! sendCommandCheckAck(pn532_packetbuffer, 3))
    return 0x0;  // no cards read

  // read data packet
  read(pn532_packetbuffer, 20);
  // check some basic stuff

  printf("Found "); 
  printf("%u",pn532_packetbuffer[7]); 
  printf(" tags\n");
  if (pn532_packetbuffer[7] != 1)
    return 0;

  uint16_t sens_res = pn532_packetbuffer[9];
  sens_res <<= 8;
  sens_res |= pn532_packetbuffer[10];
  printf("Sens Response: 0x");  
  printf("%X\n",sens_res);
  printf("Sel Response: 0x");  
  printf("%X\n",pn532_packetbuffer[11]);
  cid = 0;
  for (uint8_t i=0; i< pn532_packetbuffer[12]; i++) {
    cid <<= 8;
    cid |= pn532_packetbuffer[13+i];
    printf(" 0x"); 
    printf("%X",pn532_packetbuffer[13+i]);
  }

#ifdef PN532DEBUG
  printf("TargetID\n");
  for(uint8_t i=0;i<20;i++)
  {
    printf("%X \n",pn532_packetbuffer[i]); 
  }
#endif  
  return cid;
}

/************** low level SPI ********/
/*Function:Transmit a byte to PN532 through the SPI interface. */
inline void PN532::write(uint8_t _data) 
{
#if defined(BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
    _data = BitReverseTable256[_data];
#endif

  pn532_SPI->transfer(_data);
}

/*Function:Receive a byte from PN532 through the SPI interface */
inline uint8_t PN532::read(void) 
{
  uint8_t data_ = pn532_SPI->transfer(0);
  
#if defined(BOARD_RASPBERRY_PI_RV2) || defined(BOARD_RASPBERRY_PI_MODEL_BPLUS)
      data_ = BitReverseTable256[data_];
#endif

  return data_;
}


/************** mid level SPI */

uint8_t PN532::readSpiStatus(void) 
{
  digitalWrite(_cs, LOW);
  delay(2);
  write(PN532_SPI_STATREAD);
  uint8_t status = read();
  digitalWrite(_cs, HIGH);
  return status;
}

/************** high level SPI */
boolean PN532::checkSpiAck() 
{
  uint8_t ackbuff[6];

  read(ackbuff, 6);

  return (0 == strncmp((char *)ackbuff, (char *)pn532ack, 6));
}
/**********************************************************************/
/*Function: Read n bytes data and it stores in the parameter .        */
/*Parameter:-uint8_t* buff,saves the data read from PN532;            */
/*	    -uint8_t n,tells it wll read n bytes.                     */
/*Return:  void                                                       */
void PN532::read(uint8_t* buff, uint8_t n) 
{
  digitalWrite(_cs, LOW);
  delay(2);
  write(PN532_SPI_DATAREAD);

#ifdef PN532DEBUG
  printf("Reading: ");
#endif
  for (uint8_t i=0; i < n; i ++) 
  {
    delay(1);
    buff[i] = read();
#ifdef PN532DEBUG
    printf(" 0x");
    printf("%X",buff[i]);
#endif
  }

#ifdef PN532DEBUG
  printf("\n");
#endif

  digitalWrite(_cs, HIGH);
}

void PN532::writeCommand(uint8_t* cmd, uint8_t cmd_len) 
{
  uint8_t checksum;

  cmd_len++;

#ifdef PN532DEBUG
  printf("\nSending: ");
#endif

  digitalWrite(_cs, LOW);
  delay(2);     // or whatever the delay is for waking up the board
  write(PN532_SPI_DATAWRITE);

  checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
  write(PN532_PREAMBLE);
  write(PN532_PREAMBLE);
  write(PN532_STARTCODE2);

  write(cmd_len);
  uint8_t cmdlen_1=~cmd_len + 1;
  write(cmdlen_1);

  write(PN532_HOSTTOPN532);
  checksum += PN532_HOSTTOPN532;

#ifdef PN532DEBUG
  printf(" 0x"); 
  printf("%X",PN532_PREAMBLE);
  printf(" 0x"); 
  printf("%X",PN532_PREAMBLE);
  printf(" 0x"); 
  printf("%X",PN532_STARTCODE2);
  printf(" 0x"); 
  printf("%X",cmd_len);
  printf(" 0x"); 
  printf("%X",cmdlen_1);
  printf(" 0x"); 
  printf("%X",PN532_HOSTTOPN532);
#endif

  for (uint8_t i=0; i<cmd_len-1; i++) 
  {
    write(cmd[i]);
    checksum += cmd[i];
#ifdef PN532DEBUG
    printf(" 0x"); 
    printf("%X",cmd[i]);
#endif
  }
  uint8_t checksum_1=~checksum;
  write(checksum_1);
  write(PN532_POSTAMBLE);
  digitalWrite(_cs, HIGH);

#ifdef PN532DEBUG
  printf(" 0x"); 
  printf("%X",checksum_1);
  printf(" 0x"); 
  printf("%X",PN532_POSTAMBLE);
  printf("\n");
#endif
} 


