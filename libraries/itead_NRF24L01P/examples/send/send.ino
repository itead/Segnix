/* Send Demo */
#include <itead.h>
#include <NRF24L01P.h>

NRF24L01P rf24l01p;
uint8_t buffer[24];
uint8_t to[5] = {0x11,0x22,0x33,0x44,0x02}; /* Must be 5 Byte */
uint8_t addr[5] = {0x11,0x22,0x33,0x44,0x01}; /* Must be 24 Byte */
int8_t ret;

void setup()
{
    rf24l01p.begin(SPI,11,109,PWR_0dBm,AIR_RATE_2Mbps,50,addr);
}

void loop()
{
    static uint8_t base=0;
    base ++;
    if(base >= 10) {
        rf24l01p.end();
        exit(0);
    }
    for(int i=0;i<24;i++)
        buffer[i] = i + base;
    
    ret = rf24l01p.send_packet(to,buffer);
    //printf("ret = %d\n",ret);
    
    printf("Send(Hex):");
    for(int i=0;i<24;i++) {
        printf("%X ",buffer[i]);
    }
    printf("End\n");
    
    delay(1000);
   
}
