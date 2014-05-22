#include <PN532.h>


/*Chip select pin can be connected to D10 or D9 which is hareware optional*/
/*if you the version of NFC Shield from SeeedStudio is v2.0.*/
#define PN532_CS 109
    PN532 nfc(PN532_CS, SPI);

#define  NFC_DEMO_DEBUG 1

void setup(void) {
#ifdef NFC_DEMO_DEBUG
  printf("Hello!\n");
#endif
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
#ifdef NFC_DEMO_DEBUG
    printf("Didn't find PN53x board");
#endif
    while (1); // halt
  }
#ifdef NFC_DEMO_DEBUG
  // Got ok data, print it out!
  printf("Found chip PN5"); 
  printf("%X\n",(versiondata>>24) & 0xFF);
  printf("Firmware ver. "); 
  printf("%X",(versiondata>>16) & 0xFF);
  printf("."); 
  printf("%X\n",(versiondata>>8) & 0xFF);
  printf("Supports "); 
  printf("%X\n",versiondata & 0xFF);
#endif
  // configure board to read RFID tags and cards
  nfc.SAMConfig();
}

char DataOut[]="HELLO INITIATOR"; //16bytes
char DataIn[16];//Should be 16bytes

void loop(void) {

  // Configure PN532 as Peer to Peer Target
  if(nfc.configurePeerAsTarget()) //if connection is error-free
  {
    //trans-receive data
    if(nfc.targetTxRx(DataOut,DataIn))
    {
#ifdef NFC_DEMO_DEBUG
      printf("Data Received: ");
      printf("%s\n",DataIn);
#endif
    }
  }

}





