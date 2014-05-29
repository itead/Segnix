#include "nRF24L01P.h"

/*                              ce, cs, SPI */
nRF24L01P rf2401p(11,109,SPI);


void setup()
{
  
  rf2401p.nRF24L01_HW_Init();
  rf2401p.nRF24L01_Set_TX_Address(0x01,0x23,0x45,0x67,0x89);
  rf2401p.nRF24L01_Config(120,P0dBm,3);
  rf2401p.TX_Mode();
  
}
void loop()
{
  unsigned char send_buffer[TX_PLOAD_WIDTH];
  for(int i=0;i<TX_PLOAD_WIDTH;i++)
    send_buffer[i]=TX_PLOAD_WIDTH-i;
  printf("Send\n");
  rf2401p.nRF24L01_TxPacket(send_buffer);
  delay(1000);
}


