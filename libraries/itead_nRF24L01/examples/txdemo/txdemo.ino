#include "nRF24L01P.h"

nRF24L01P rf2401p;


void setup()
{
  
  rf2401p.nRF24L01_HW_Init();
  rf2401p.nRF24L01_Set_TX_Address(0x01,0x23,0x45,0x67,0x89);
  rf2401p.nRF24L01_Config(120,P0dBm,3);
  rf2401p.TX_Mode();
  
}
void loop()
{
  unsigned char tmp[5]={'L','i','n','u','x'};
  printf("Send Linux\n");
  rf2401p.nRF24L01_TxPacket(tmp);
  delay(1000);
}


