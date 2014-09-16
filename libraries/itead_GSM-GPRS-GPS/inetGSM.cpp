#include "inetGSM.h"
#define _GSM_CONNECTION_TOUT_ 5
#define _TCP_CONNECTION_TOUT_ 20
#define _GSM_DATA_TOUT_ 10

#if 1 // add by wpf
InetGSM::InetGSM(SIMCOM900 & gsm_):_gsm(&gsm_)
{
}
#endif

int InetGSM::httpGET(const char* server, int port, const char* path, char* result, int resultlength)
{
     boolean connected=false;
     int n_of_at=0;
     int length_write;
     char end_c[2];
     end_c[0]=0x1a;
     end_c[1]='\0';

     /*
     Status = ATTACHED.
     if(gsm.getStatus()!=GSM::ATTACHED)
       return 0;
     */
     while(n_of_at<3) {
          if(!connectTCP(server, port)) {
#ifdef DEBUG_ON
               printf("DB:NOT CONN\n");
#endif
               n_of_at++;
          } else {
               connected=true;
               n_of_at=3;
          }
     }

     if(!connected) return 0;

     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("GET ");
     __gsm.SimpleWrite(path);
     __gsm.SimpleWrite(" HTTP/1.0\r\nHost: ");
     __gsm.SimpleWrite(server);
     __gsm.SimpleWrite("\r\n");
     __gsm.SimpleWrite("User-Agent: Arduino");
     __gsm.SimpleWrite("\r\n\r\n");
     __gsm.SimpleWrite(end_c);

     switch(__gsm.WaitResp(10000, 10, "SEND OK")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }


     delay(50);
#ifdef DEBUG_ON
     printf("DB:SENT\n");
#endif
     int res = __gsm.read(result, resultlength);

     //gsm.disconnectTCP();

     //int res=1;
     return res;
}

int InetGSM::httpPOST(const char* server, int port, const char* path, const char* parameters, char* result, int resultlength)
{
     boolean connected=false;
     int n_of_at=0;
     char itoaBuffer[8];
     int num_char;
     char end_c[2];
     end_c[0]=0x1a;
     end_c[1]='\0';

     while(n_of_at<3) {
          if(!connectTCP(server, port)) {
#ifdef DEBUG_ON
               printf("DB:NOT CONN\n");
#endif
               n_of_at++;
          } else {
               connected=true;
               n_of_at=3;
          }
     }

     if(!connected) return 0;

     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("POST ");
     __gsm.SimpleWrite(path);
     __gsm.SimpleWrite(" HTTP/1.1\r\nHost: ");
     __gsm.SimpleWrite(server);
     __gsm.SimpleWrite("\r\n");
     __gsm.SimpleWrite("User-Agent: Arduino\r\n");
     __gsm.SimpleWrite("Content-Type: application/x-www-form-urlencoded\r\n");
     __gsm.SimpleWrite("Content-Length: ");
     #if 0 // disabled by wpf
     itoa(strlen(parameters),itoaBuffer,10);
     #else
     sprintf(itoaBuffer,"%d",strlen(parameters));
     #endif
     __gsm.SimpleWrite(itoaBuffer);
     __gsm.SimpleWrite("\r\n\r\n");
     __gsm.SimpleWrite(parameters);
     __gsm.SimpleWrite("\r\n\r\n");
     __gsm.SimpleWrite(end_c);

     switch(__gsm.WaitResp(10000, 10, "SEND OK")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }

     delay(50);
#ifdef DEBUG_ON
     printf("DB:SENT\n");
#endif

     int res= __gsm.read(result, resultlength);
     //gsm.disconnectTCP();
     return res;
}

int InetGSM::openmail(char* server, char* loginbase64, char* passbase64, char* from, char* to, char* subj)
{
     boolean connected=false;
     int n_of_at=0;
     char end_c[2];
     end_c[0]=0x1a;
     end_c[1]='\0';

     while(n_of_at<3) {
          if(!connectTCP(server, 25)) {
#ifdef DEBUG_ON
               printf("DB:NOT CONN\n");
#endif
               n_of_at++;
          } else {
               connected=true;
               n_of_at=3;
          }
     }

     if(!connected) return 0;

     delay(100);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("HELO ");
     __gsm.SimpleWrite(server);
     __gsm.SimpleWrite("\n");
     __gsm.SimpleWrite(end_c);
     __gsm.WaitResp(5000, 100, "OK");
     if(!__gsm.IsStringReceived("SEND OK"))
          return 0;
     delay(500);
     __gsm.WaitResp(5000, 100);

     delay(100);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSEND");
     switch(__gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("AUTH LOGIN\n");
     __gsm.SimpleWrite(end_c);
     __gsm.WaitResp(5000, 100, "OK");
     if(!__gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     __gsm.WaitResp(5000, 100);

     delay(100);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSEND");
     switch(__gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite(loginbase64);
     __gsm.SimpleWrite("\n");
     __gsm.SimpleWrite(end_c);
     __gsm.WaitResp(5000, 100, "OK");
     if(!__gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     __gsm.WaitResp(5000, 100);

     delay(100);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSEND");
     switch(__gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite(passbase64);
     __gsm.SimpleWrite("\n");
     __gsm.SimpleWrite(end_c);
     __gsm.WaitResp(5000, 100, "OK");
     if(!__gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     __gsm.WaitResp(5000, 100);


     delay(100);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSEND");
     switch(__gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("MAIL From: <");
     __gsm.SimpleWrite(from);
     __gsm.SimpleWrite(">\n");
     __gsm.SimpleWrite(end_c);
     __gsm.WaitResp(5000, 100, "OK");
     if(!__gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     __gsm.WaitResp(5000, 100, "");

     delay(100);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSEND");
     switch(__gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("RCPT TO: <");
     __gsm.SimpleWrite(to);
     __gsm.SimpleWrite(">\n");
     __gsm.SimpleWrite(end_c);
     __gsm.WaitResp(5000, 100, "OK");
     if(!__gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     __gsm.WaitResp(5000, 100, "");

     delay(100);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSEND");
     switch(__gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("Data\n");
     __gsm.SimpleWrite(end_c);
     __gsm.WaitResp(5000, 100, "OK");
     if(!__gsm.IsStringReceived("OK"))
          return 0;
     delay(500);
     __gsm.WaitResp(5000, 100, "");

     delay(100);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSEND");
     switch(__gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("Subject: ");
     __gsm.SimpleWrite(subj);
     __gsm.SimpleWrite("\n\n");

     return 1;
}
int InetGSM::closemail()
{
     char end_c[2];
     end_c[0]=0x1a;
     end_c[1]='\0';
     
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("\n.\n");
     __gsm.SimpleWrite(end_c);
     disconnectTCP();
     return 1;
}


int InetGSM::attachGPRS(char* domain, char* dom1, char* dom2)
{
     int i=0;
     delay(5000);

     //gsm._tf.setTimeout(_GSM_DATA_TOUT_);	//Timeout for expecting modem responses.
     __gsm.WaitResp(50, 50);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIFSR");
     if(__gsm.WaitResp(5000, 50, "ERROR")!=RX_FINISHED_STR_RECV) {
#ifdef DEBUG_ON
          printf("DB:ALREADY HAVE AN IP\n");
#endif
          __gsm.flush(); // add by wpf
          __gsm.SimpleWriteln("AT+CIPCLOSE");
          __gsm.WaitResp(5000, 50, "ERROR");
          delay(2000);
          __gsm.flush(); // add by wpf
          __gsm.SimpleWriteln("AT+CIPSERVER=0");
          __gsm.WaitResp(5000, 50, "ERROR");
          return 1;
     } else {

#ifdef DEBUG_ON
          printf("DB:STARTING NEW CONNECTION\n");
#endif
          __gsm.flush(); // add by wpf
          __gsm.SimpleWriteln("AT+CIPSHUT");

          switch(__gsm.WaitResp(500, 50, "SHUT OK")) {

          case RX_TMOUT_ERR:
               return 0;
               break;
          case RX_FINISHED_STR_NOT_RECV:
               return 0;
               break;
          }
#ifdef DEBUG_ON
          printf("DB:SHUTTED OK\n");
#endif
          delay(1000);
          __gsm.flush(); // add by wpf
          __gsm.SimpleWrite("AT+CSTT=\"");
          __gsm.SimpleWrite(domain);
          __gsm.SimpleWrite("\",\"");
          __gsm.SimpleWrite(dom1);
          __gsm.SimpleWrite("\",\"");
          __gsm.SimpleWrite(dom2);
          __gsm.SimpleWrite("\"\r");


          switch(__gsm.WaitResp(500, 50, "OK")) {

          case RX_TMOUT_ERR:
               return 0;
               break;
          case RX_FINISHED_STR_NOT_RECV:
               return 0;
               break;
          }
#ifdef DEBUG_ON
          printf("DB:APN OK\n");
#endif
          delay(5000);
          __gsm.flush(); // add by wpf
          __gsm.SimpleWriteln("AT+CIICR");

          switch(__gsm.WaitResp(10000, 50, "OK")) {
          case RX_TMOUT_ERR:
               return 0;
               break;
          case RX_FINISHED_STR_NOT_RECV:
               return 0;
               break;
          }
#ifdef DEBUG_ON
          printf("DB:CONNECTION OK\n");
#endif

          delay(1000);

          __gsm.flush(); // add by wpf
          __gsm.SimpleWriteln("AT+CIFSR");
          if(__gsm.WaitResp(5000, 50, "ERROR")!=RX_FINISHED_STR_RECV) {
#ifdef DEBUG_ON
               printf("DB:ASSIGNED AN IP\n");
#endif
               __gsm.setStatus(__gsm.ATTACHED);
               return 1;
          }
#ifdef DEBUG_ON
          printf("DB:NO IP AFTER CONNECTION\n");
#endif
          return 0;
     }
}

int InetGSM::dettachGPRS()
{
     if (__gsm.getStatus()==__gsm.IDLE) return 0;

     //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);

     //_cell.flush();

     //GPRS dettachment.
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CGATT=0");
     if(__gsm.WaitResp(5000, 50, "OK")!=RX_FINISHED_STR_NOT_RECV) {
          __gsm.setStatus(__gsm.ERROR);
          return 0;
     }
     delay(500);

     // Commented in initial trial code!!
     //Stop IP stack.
     //_cell << "AT+WIPCFG=0" <<  _DEC(cr) << endl;
     //	if(!gsm._tf.find("OK")) return 0;
     //Close GPRS bearer.
     //_cell << "AT+WIPBR=0,6" <<  _DEC(cr) << endl;

     __gsm.setStatus(__gsm.READY);
     return 1;
}

int InetGSM::connectTCP(const char* server, int port)
{
     //gsm._tf.setTimeout(_TCP_CONNECTION_TOUT_);

     //Status = ATTACHED.
     //if (getStatus()!=ATTACHED)
     //return 0;

     //_cell.flush();

     //Visit the remote TCP server.
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("AT+CIPSTART=\"TCP\",\"");
     __gsm.SimpleWrite(server);
     __gsm.SimpleWrite("\",");
     __gsm.SimpleWriteln(port);

     switch(__gsm.WaitResp(1000, 200, "OK")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }
#ifdef DEBUG_ON
     printf("DB:RECVD CMD\n");
#endif
     if (!__gsm.IsStringReceived("CONNECT OK")) {
          switch(__gsm.WaitResp(15000, 200, "OK")) {
          case RX_TMOUT_ERR:
               return 0;
               break;
          case RX_FINISHED_STR_NOT_RECV:
               return 0;
               break;
          }
     }

#ifdef DEBUG_ON
     printf("DB:OK TCP\n");
#endif

     delay(3000);
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSEND");
     switch(__gsm.WaitResp(5000, 200, ">")) {
     case RX_TMOUT_ERR:
          return 0;
          break;
     case RX_FINISHED_STR_NOT_RECV:
          return 0;
          break;
     }

#ifdef DEBUG_ON
     printf("DB:>\n");
#endif
     delay(4000);
     return 1;
}

int InetGSM::disconnectTCP()
{
     //Status = TCPCONNECTEDCLIENT or TCPCONNECTEDSERVER.
     /*
     if ((getStatus()!=TCPCONNECTEDCLIENT)&&(getStatus()!=TCPCONNECTEDSERVER))
        return 0;
     */
     //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);


     //_cell.flush();

     //Switch to AT mode.
     //_cell << "+++" << endl;

     //delay(200);

     //Close TCP client and deact.
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPCLOSE");

     //If remote server close connection AT+QICLOSE generate ERROR
     /*if(gsm._tf.find("OK"))
     {
       if(getStatus()==TCPCONNECTEDCLIENT)
         gsm.setStatus(ATTACHED);
       else
         gsm.setStatus(TCPSERVERWAIT);
       return 1;
     }
     gsm.setStatus(ERROR);

     return 0;    */
     if(__gsm.getStatus()==__gsm.TCPCONNECTEDCLIENT)
          __gsm.setStatus(__gsm.ATTACHED);
     else
          __gsm.setStatus(__gsm.TCPSERVERWAIT);
     return 1;
}

int InetGSM::connectTCPServer(int port)
{
     /*
       if (getStatus()!=ATTACHED)
          return 0;
     */
     //gsm._tf.setTimeout(_GSM_CONNECTION_TOUT_);

     //_cell.flush();

     // Set port
     __gsm.flush(); // add by wpf
     __gsm.SimpleWrite("AT+CIPSERVER=1,");
     __gsm.SimpleWriteln(port);
     /*
       switch(gsm.WaitResp(5000, 50, "OK")){
     	case RX_TMOUT_ERR:
     		return 0;
     	break;
     	case RX_FINISHED_STR_NOT_RECV:
     		return 0;
     	break;
       }

       switch(gsm.WaitResp(5000, 50, "SERVER")){ //Try SERVER OK
     	case RX_TMOUT_ERR:
     		return 0;
     	break;
     	case RX_FINISHED_STR_NOT_RECV:
     		return 0;
     	break;
       }
     */
     //delay(200);

     return 1;

}

boolean InetGSM::connectedClient()
{
     /*
     if (getStatus()!=TCPSERVERWAIT)
        return 0;
     */
     __gsm.flush(); // add by wpf
     __gsm.SimpleWriteln("AT+CIPSTATUS");
     // Alternative: AT+QISTAT, although it may be necessary to call an AT
     // command every second,which is not wise
     /*
     switch(gsm.WaitResp(1000, 200, "OK")){
     case RX_TMOUT_ERR:
     	return 0;
     break;
     case RX_FINISHED_STR_NOT_RECV:
     	return 0;
     break;
     }*/
     //gsm._tf.setTimeout(1);
     if(__gsm.WaitResp(5000, 50, "CONNECT OK")!=RX_FINISHED_STR_RECV) {
          __gsm.setStatus(__gsm.TCPCONNECTEDSERVER);
          return true;
     } else
          return false;
}

