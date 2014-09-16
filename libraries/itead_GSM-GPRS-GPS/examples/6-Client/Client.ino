#include "SIM900.h"
#include "inetGSM.h"
//Simple sketch to start a connection as client.
#define GSM_POWER_PIN 11
static SIMCOM900 gsm(GSM_POWER_PIN,Serial2);
InetGSM inet(gsm);

char msg[50];
int numdata;
char inSerial[50];
int i=0;
boolean started=false;

void setup()
{
     //Serial connection.
     printf("GSM Shield GPRS Client Test\n");
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     printf("Start...\n");
     if (gsm.begin(9600)) {
          printf("\nstatus=READY\n");
          started=true;
     } else printf("\nstatus=IDLE\n");

     if(started) {
          //GPRS attach, put in order APN, username and password.
          //If no needed auth let them blank.
          if (inet.attachGPRS("internet.wind", "", ""))
               printf("status=ATTACHED\n");
          else printf("status=ERROR\n");
          delay(1000);

          //Read IP address.
          gsm.SimpleWriteln("AT+CIFSR");
          delay(5000);
          //Read until serial buffer is empty.
          gsm.WhileSimpleRead();

          //TCP Client GET, send a GET request to the server and
          //save the reply.
          numdata=inet.httpGET("www.baidu.com", 80, "/", msg, 50);
          //Print the results.
          printf("\nNumber of data received:");
          printf("%d\n",numdata);
          printf("\nData received:");
          printf("%s\n",msg);
     }
};

void loop()
{
};

