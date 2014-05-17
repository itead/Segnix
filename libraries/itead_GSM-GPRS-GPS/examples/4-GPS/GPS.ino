#include "SIM900.h"
#include "gps.h"

#define GSM_POWER_PIN 11
static SIMCOM900 gsm(GSM_POWER_PIN,Serial2);
static GPSGSM gps(gsm);

static char lon[15];
static char lat[15];
static char alt[15];
static char time_[20];
static char vel[15];
static char msg1[5];
static char msg2[5];

static char stat;
static char inSerial[20];
static int i=0;
static boolean started=false;

void setup()
{
     printf("GSM Shield GPS Test\n");
     printf("Start...\n");
     if (gsm.begin(9600)) {
         printf("\nstatus=READY\n");
          gsm.forceON();	//To ensure that SIM908 is not only in charge mode
          started=true;
     } else printf("\nstatus=IDLE\n");

     if(started) {
          //GPS attach
          if (gps.attachGPS())
               printf("status=GPSREADY\n");
          else printf("status=ERROR\n");
          delay(20000);	//Time for fixing
          stat=gps.getStat();
          if(stat==1)
               printf("NOT FIXED\n");
          else if(stat==0)
               printf("GPS OFF\n");
          else if(stat==2)
               printf("2D FIXED\n");
          else if(stat==3)
               printf("3D FIXED\n");
          delay(5000);
          //Get data from GPS
          gps.getPar(lon,lat,alt,time_,vel);
          printf("LON=%s\n",lon);
          printf("LAT=%s\n",lat);
          printf("ALT=%s\n",alt);
          printf("TIME=%s\n",time_);
          printf("VEL=%s\n",vel);
     }
};

void loop()
{
};
