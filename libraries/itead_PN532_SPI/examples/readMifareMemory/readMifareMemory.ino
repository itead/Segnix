//This example reads a MIFARE memory block. It is tested with a new MIFARE 1K cards. Uses default keys.
//Contributed by Seeed Technology Inc (www.seeedstudio.com)

#include <PN532.h>

/*Chip select pin can be connected to D10 or D9 which is hareware optional*/
/*if you the version of NFC Shield from SeeedStudio is v2.0.*/
#define PN532_CS 109

PN532 nfc(PN532_CS,SPI);
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

void loop(void) {
  uint32_t id;
  // look for MiFare type cards
  id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);

  if (id != 0) 
  {
#ifdef NFC_DEMO_DEBUG
    printf("\nRead card #%u\n",id);
#endif  
    uint8_t keys[]= { 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    if(nfc.authenticateBlock(1, id ,0x08,KEY_A,keys)) //authenticate block 0x08
    {
      //if authentication successful
      uint8_t block[16];
      //read memory block 0x08
      if(nfc.readMemoryBlock(1,0x08,block))
      {
#ifdef NFC_DEMO_DEBUG
        //if read operation is successful
        for(uint8_t i=0;i<16;i++)
        {
          //print memory block
          printf("%X ",block[i]);
        }
        printf("\n");
#endif
      }
    }
  }
 delay(500);
}


