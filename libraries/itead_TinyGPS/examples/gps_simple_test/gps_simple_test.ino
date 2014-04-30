
#include <itead_TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 3(rx) and 4(tx).
*/

TinyGPS gps;

void setup()
{
  
  Serial.begin(9600);
  
  printf("Simple TinyGPS library v. "); 
  printf("%d\n",TinyGPS::library_version());
  printf("by Mikal Hart\n");
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
       printf("%c",c);
      // Serial3.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    printf("\nLAT=");
    printf("%6f",flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat);
    printf(" LON=");
    printf("%6f",flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon);
    printf(" SAT=");
    printf("%u",gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    printf(" PREC=");
    printf("%u",gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
  printf("\n CHARS=%u",chars);
  printf(" SENTENCES=%u",sentences);
  printf(" CSUM ERR=%u\n",failed);

}
