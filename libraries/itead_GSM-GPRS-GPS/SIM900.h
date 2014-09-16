#ifndef SIMCOM900_H
#define SIMCOM900_H

#include "GSM.h"

class SIMCOM900 : public virtual GSM {

private:
     int configandwait(char* pin);
     int setPIN(char *pin);
     int changeNSIPmode(char);

public:
     #if 0 // disabled by wpf
     SIMCOM900();
     #endif
     #if 1 // add by wpf
     SIMCOM900(uint16_t gsm_on, Serial_ &Serial);
     SIMCOM900(uint16_t gsm_on, uint16_t gsm_rst, Serial_ &Serial);
     #endif
     ~SIMCOM900();
     
     #if 1 // add by wpf
     void powerOn();
     void powerDown();
     void powerReset();
     #endif
     int getCCI(char* cci);
     int getIMEI(char* imei);
     int sendSMS(const char* to, const char* msg);
     boolean readSMS(char* msg, int msglength, char* number, int nlength);
     boolean readCall(char* number, int nlength);
     boolean call(char* number, unsigned int milliseconds);
     char forceON();
     virtual int read(char* result, int resultlength);
     virtual uint8_t read();
     virtual int available();
     int readCellData(int &mcc, int &mnc, long &lac, long &cellid);
     void SimpleRead();
     void WhileSimpleRead();
     void SimpleWrite(char *comm);
     void SimpleWrite(char const *comm);
     void SimpleWrite(int comm);
     #if 0 // disabled 
     void SimpleWrite(const __FlashStringHelper *pgmstr);
     #endif
     void SimpleWriteln(char *comm);
     void SimpleWriteln(char const *comm);
     #if 0
     void SimpleWriteln(const __FlashStringHelper *pgmstr);
     #endif
     void SimpleWriteln(int comm);
};

#if 0 // disabled by wpf
extern SIMCOM900 __gsm;
#endif

#endif

