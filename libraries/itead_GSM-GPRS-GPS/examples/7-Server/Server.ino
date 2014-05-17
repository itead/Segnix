#include "SIM900.h"
#include "inetGSM.h"

//Simple sketch to start a connection as server.
#define GSM_POWER_PIN 11
static SIMCOM900 gsm(GSM_POWER_PIN,Serial2);
InetGSM inet(gsm);

char msg[50];
int numdata;
char inSerial[50];
int i=0;
boolean started=false;
long lasttime=millis();

void setup()
{
     //Serial connection.
     printf("GSM Shield GPRS Test\n");
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
          int i=0;
          while(i<20) {
               gsm.SimpleRead();
               i++;
          }

          //TCP Server. Start the socket connection
          //as server on the assigned port.
          printf("%s\n",msg);
          delay(5000);
          if (inet.connectTCPServer(80))
               printf("status=TCPSERVERWAIT\n");
          else printf("ERROR in Server\n");
          lasttime=millis();
     }
};


void loop()
{
     if(started) {
          //Check if there is an active connection.
          if (inet.connectedClient()) {
               //Read and print the last message received.
               gsm.read(msg, 50);
               printf("%s\n",msg);
          }
     } 
};

