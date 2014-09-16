//Simple sketch to delete/get/write phone number.
#include <SIM900.h>

static SIMCOM900 gsm(11,Serial2);

boolean started=false;
int pos=0;
char n[20];
int i=0;
int ret;

void setup()
{
     //Serial connection.
     printf("GSM Shield PhoneBook Test\n");
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     printf("Start...\n");
     if (gsm.begin(9600)) {
          printf("\nstatus=READY\n");
          started=true;
     } else printf("\nstatus=IDLE\n");
     
     if(!started) {
        printf("setup error,Halt now...\n");
        while(1)delay(10000);
     }
    
};

void loop()
{   
    printf("Delete all the phone number\n");
    for(i=1;i<=20;i++) {
        ret = gsm.DelPhoneNumber(i);
        printf("Delete Pos= %d, gsm.DelPhoneNUmber ret= %d\n",i,ret);
    }

     printf("Write all the phone number\n");
    for(i=1;i<=20;i++) {
        sprintf(n,"%d",i+1000);
        ret = gsm.WritePhoneNumber(i,n);
        printf("Write Pos= %d, gsm.WritePhoneNUmber ret= %d\n",i,ret);
    }

     printf("Get all the phone number\n");
    for(i=1;i<=20;i++) {
        ret = gsm.GetPhoneNumber(i,n);
        printf("Get Pos= %d, gsm.GetPhoneNUmber ret= %d\n",i,ret);
        printf("Number: %s\n",n);
        sprintf(n,"%d",i+1000);
        if( 1 != gsm.ComparePhoneNumber(i,n)){
            printf("Error compare. Pos=%d\n",i);
        }
    }
    
     printf("Now,halt...\n");
     while(1)delay(10000);
};
