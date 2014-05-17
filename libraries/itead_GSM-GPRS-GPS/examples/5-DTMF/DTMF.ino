#include "SIM900.h"
#include "sms.h"
#include "call.h"


//Simple sketch to detect DTMF tones during a call

//We have to create the classes for calls.
#define GSM_POWER_PIN 11
SIMCOM900 gsm(GSM_POWER_PIN,Serial2);
CallGSM call(gsm);

char number[20];
byte stat=0;
int value=0;
int pin=10; /* no used ??? */
char value_str[5];
char DTMF_char='_';
int count=0;

void setup()
{
     pinMode(pin,INPUT);
     //Serial connection.
     printf("GSM Shield DTMF Test\n");
     printf("Start...\n");
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin(9600))
          printf("\nstatus=READY\n");
     else printf("\nstatus=IDLE\n");
     //Enable DTMF detection for SIM900
     call.SetDTMF(1);
};

void loop()
{
     //Chekcs status of call
     stat=call.CallStatus();
     //If the incoming call is from an authorized number
     //saved on SIM in the positions range from 1 to 3.
     if(stat==CALL_INCOM_VOICE) {
          printf("Pick Up\n");
          delay(50);
          call.PickUp();
     }
     while(stat==CALL_ACTIVE_VOICE) {
          for (int k=0; k<100; k++) {
               DTMF_char=call.DetDTMF();
               if(DTMF_char!='-')
                    printf("%c",DTMF_char);
          }
          stat=call.CallStatus();

     }
     delay(1000);
};
