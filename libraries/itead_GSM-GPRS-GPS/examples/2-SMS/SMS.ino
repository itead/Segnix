//Simple sketch to delete/get/write phone number.
#include "SIM900.h"
#include "sms.h"

static SIMCOM900 gsm(11,Serial2);
static SMSGSM sms(gsm);

boolean started=false;
int pos=0;
char number[20];
char sms_buffer[200];
int ret;
int i;

void setup()
{
     //Serial connection.
     printf("GSM Shield SMS Test\n");
     printf("Start...\n");
     if (gsm.begin(9600)) {
          printf("\nstatus=READY\n");
          started=true;
     } else printf("\nstatus=IDLE\n");
     
     if(!started) {
        printf("setup error,Halt now...");
        while(1)delay(10000);
     }

#if 0   
    printf("Delete all messages\n");
    for(i=1;i<=20;i++) {
        ret = sms.DeleteSMS(i);
        printf("Delete Pos= %d, sms.DeleteSMS ret= %d\n",i,ret);
    }
#endif
#if 0
    ret = sms.SendSMS("10086","101");
    if ( ret == 1) {
        printf("Send OK\n");
        delay(5000);
    } else {
        printf("Send ERR. ret = %d\n",ret);
        delay(3000);
    }
#endif        
};

void loop()
{   
    pos = sms.IsSMSPresent(SMS_UNREAD);
    
    if(pos > 0) {
        sms.GetSMS(pos,number,sms_buffer,sizeof(sms_buffer));
        printf("Pos: %d\n",pos);
        printf("From: %s\n",number);
        printf("Content: %s\n",sms_buffer);
    } else {
        printf("sms.IsSMSPresent(SMS_UNREAD) ret= %d\n",pos);
    }
    delay(1000);
};
