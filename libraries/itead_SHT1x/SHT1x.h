/**
 * SHT1x Library
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au> / <www.practicalarduino.com>
 * Based on previous work by:
 *    Maurice Ribble: <www.glacialwanderer.com/hobbyrobotics/?p=5>
 *    Wayne ?: <ragingreality.blogspot.com/2008/01/ardunio-and-sht15.html>
 *
 * Manages communication with SHT1x series (SHT10, SHT11, SHT15)
 * temperature / humidity sensors from Sensirion (www.sensirion.com).
 */
#ifndef __SHT1X_H__
#define __SHT1X_H__

#include <itead.h>
#include <Dev_THSensorInterface.h>

/**
 * SHT1x implements THSensorInterface. 
 *
 */
class SHT1x : public THSensorInterface
{
public: /* Implementation of methods in THSensorInterface */
    virtual int32_t begin(void)
    {
        /* Nothing to do */
        return 0;
    }
    virtual int32_t getData(float *temp_c, float *temp_f, float *hum)
    {
        if (temp_c) 
            *temp_c = readTemperatureC();
        if (temp_f)
            *temp_f = readTemperatureF();
        if (hum)
            *hum = readHumidity();
        return 0;
    }
    virtual int32_t end(void)
    {
        /* Nothing to do */
        return 0;
    }
    
public: /* Constructors */
    SHT1x(int dataPin, int clockPin);

    float readHumidity();
    float readTemperatureC();
    float readTemperatureF();
    
  private:
    int _dataPin;
    int _clockPin;
    int _numBits;
    float readTemperatureRaw();
    int shiftIn(int _dataPin, int _clockPin, int _numBits);    
    void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
    void sendCommandSHT(int _command, int _dataPin, int _clockPin);
    void waitForResultSHT(int _dataPin);
    int getData16SHT(int _dataPin, int _clockPin);
    void skipCrcSHT(int _dataPin, int _clockPin);
};

#endif /* #ifndef __SHT1X_H__ */
