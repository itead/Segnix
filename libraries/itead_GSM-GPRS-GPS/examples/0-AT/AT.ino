#include "SIM900.h"

SIMCOM900 gsm(11,Serial2);

int numdata;
char inSerial[4096];
int i=0;
int cnt=0;


void receive_response(uint16_t wait,uint16_t no_attempt, uint16_t interval)
{
    uint16_t cnt=0;
    
    delay(wait);
    printf("\n[");
    for(cnt=0;cnt<no_attempt;cnt++) {
        delay(interval);
        while(gsm.available()>0) {
            printf("%c",gsm.read());
        }
    }
    printf("]\n");
}

void setup()
{
     //Serial connection.
     printf("GSM Shield AT Test\n");
     //Start configuration of shield with baudrate.
     printf("Start...\n");
     if (gsm.begin(9600))
          printf("\nstatus=READY\n");
     else printf("\nstatus=IDLE\n");

};


void loop()
{
    
    printf("Send Cmd: AT\n");
    gsm.SimpleWriteln("AT"); 
    receive_response(100,2,100);
    
    printf("Send Cmd: ATE0\n");
    gsm.SimpleWriteln("ATE0");
    receive_response(100,2,100);
    
    printf("Send Cmd: ATE1\n");
    gsm.SimpleWriteln("ATE1");
    receive_response(100,2,100);

    delay(1000);
};

