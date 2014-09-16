/*
  LOG.cpp - Library for standard logging convention.
  Created by Meir Michanie, June 9, 2010.
  Released into the public domain.
  Version 0.1
*/

#include "LOG.h"

LOG::LOG(int level)
{
     setLevel(level);
}

void LOG::DATA(const char* string)
{
     if (_level > 4) {
          printf("%s",string);
     }
}

void LOG::DATA(int number)
{
     if (_level > 4) {
          printf("%d",number);
     }
}

void LOG::DEBUG(const char* string)
{
     if (_level > 3) {
          printf("\n[DEBUG]: ");
          printf("%s\n",string);
     }
}

void LOG::INFO(const char* string)
{
     if (_level > 2) {
          printf("\n[INFO]: ");
          printf("%s\n",string);
     }
}

void LOG::WARNING(const char* string)
{
     if (_level > 1) {
          printf("\n[WARNING]: ");
          printf("%s\n",string);
     }
}

void LOG::CRITICAL(const char* string)
{
     if (_level > 0) {
          printf("\n[CRITICAL]: ");
          printf("%s\n",string);
     }
}
