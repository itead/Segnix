#include <SIM900.h>
#include <sms.h>
#include <call.h>

#define GSM_POWER_PIN 11
SIMCOM900 gsm(GSM_POWER_PIN,Serial2);
SMSGSM sms(gsm);
CallGSM call(gsm);

boolean started=false;
int pos=0;
char number[20];
char sms_buffer[200];
int ret;
int i;

void setup()
{
     //Serial connection.
     printf("GSM Shield Call Test\n");
     printf("Start...\n");
     if (gsm.begin(9600)) {
          printf("\nstatus=READY\n");
          started=true;
     } else printf("\nstatus=IDLE\n");
     
     if(!started) {
        printf("setup error,Halt now...");
        while(1)delay(10000);
     }

    call.Call("10086");
    delay(20000);
    call.HangUp();
    delay(2000);
};

void loop()
{   
    
    ret = call.CallStatus();
    if( ret == CALL_INCOM_VOICE ){
        printf("Hand Up You !\n");
        call.HangUp();
        delay(5000);
    }
    delay(2000);
};
