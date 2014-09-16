/* Recv Demo */
#include <itead.h>
#include <NRF24L01P.h>

NRF24L01P rf24l01p;
uint8_t buffer[24];
uint8_t from[5] = {0};  /* Must be 5 Byte */
uint8_t addr[5] = {0x11,0x22,0x33,0x44,0x02}; /* Must be 24 Byte */
int8_t ret;

void setup()
{
    rf24l01p.begin(11,109,PWR_0dBm,AIR_RATE_2Mbps,50,addr);
}

void loop()
{
    ret = rf24l01p.recv_packet(from,buffer);
    //printf("ret = %d\n",ret);
    printf("From:");
    for(int i=0;i<5;i++)
        printf("%X ",from[i]);
    
    printf("\nRecv(Hex):");
    for(int i=0;i<24;i++) {
        printf("%X ",buffer[i]);
    }
    printf("End\n");
}

