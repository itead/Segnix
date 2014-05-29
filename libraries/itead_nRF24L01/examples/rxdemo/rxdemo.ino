#include "nRF24L01P.h"
/*                              ce, cs, SPI */
nRF24L01P rf2401p(11,109,SPI);

void setup()
{
    printf("setup begin()\n");
    rf2401p.nRF24L01_HW_Init();
    rf2401p.nRF24L01_Set_RX_Address(0x01,0x23,0x45,0x67,0x89);
    rf2401p.nRF24L01_Config(120,P0dBm,3);
    rf2401p.RX_Mode();
    printf("setup end()\n");  
}
void loop()
{
    unsigned char rec_buffer[RX_PLOAD_WIDTH]={0};
    if(rf2401p.nRF24L01_RxPacket(rec_buffer))
    {
        printf("Received(HEX):\n");
        for(int i=0;i<RX_PLOAD_WIDTH;i++)
            printf("%02X ",rec_buffer[i]);
        printf("\n");
    }
    
}
