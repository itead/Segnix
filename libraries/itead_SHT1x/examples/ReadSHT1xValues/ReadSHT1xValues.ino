/**
 * ReadSHT1xValues
 *
 * Read temperature and humidity values from an SHT1x-series (SHT10,
 * SHT11, SHT15) sensor.
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au>
 * www.practicalarduino.com
 */

#include <SHT1x.h>

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  68
#define clockPin 67
SHT1x sht1x(dataPin, clockPin);

void setup()
{
   printf("Starting up\n");
}

void loop()
{
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();

  // Print the values to the serial port
  printf("Temperature: ");
  printf("%.2f",temp_c);
  printf("C / ");
  printf("%.2f",temp_f);
  printf("F. Humidity: ");
  printf("%.2f",humidity);
  printf("\%\n");

  delay(2000);
}
