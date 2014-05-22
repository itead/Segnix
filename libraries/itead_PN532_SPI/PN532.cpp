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
// IteadOS SDK <https://github.com/iteadsw/SDK>.
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
  pn532_SPI->setClockDivider(SPI_CLOCK_DIV16);
  digitalWrite(_cs, LOW);
  delay(1000); 
  /*Not exactly sure why but we have to send a dummy command to get synced up*/
  pn532_packetbuffer[0] = PN532_FIRMWAREVERSION;
  /*Ignore response!*/
  sendCommandCheckAck(pn532_packetbuffer, 1);
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
  pn532_SPI->transfer(_data);
}

/*Function:Receive a byte from PN532 through the SPI interface */
inline uint8_t PN532::read(void) 
{
  uint8_t data_ = pn532_SPI->transfer(0);
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


