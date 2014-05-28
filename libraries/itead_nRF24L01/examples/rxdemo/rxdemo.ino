#include "nRF24L01P.h"

nRF24L01P rf2401p;
//nRF24L01P rf2401p(SPI);

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
    unsigned char tmp[5]={0};
    if(rf2401p.nRF24L01_RxPacket(tmp))
    {
        tmp[4]='\0';
        printf("Received:%s\n",(char *)tmp);
    }
    delay(1000);
}
