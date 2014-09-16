/**
 * @file itead_print.h
 *
 * Copy of Print.h and modified by Wu Pengfei.
 */

/*
  itead_print.h - copy of Print.h and modified by Wu Pengfei.

  Print.h - Base class that provides print() and println()
  Copyright (c) 2008 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __ITEAD_PRINT_H__
#define __ITEAD_PRINT_H__
#ifdef __cplusplus

#include <inttypes.h>
#include <stdio.h> // for size_t
#include <string.h>
#include <stdint.h>

#include <string>
using std::string;

#if 0 // replaced by Print.SCALE
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#endif

/**
 * Abstract class provides protable methods for print message and variable with format. 
 *
 * Subclass of Print must implements pure virtual method "virtual size_t write(uint8_t) = 0;". 
 * Thus, print and println methods here can be used to output some message/number with specific format.
 * The output of print and println methods is related to the implementation of pure virtual method "write".
 * class Serial_, for example, inherits from Print and implements the "write" method with UART communication
 * protocol. So the output will be shown on Serial Console or other terminals. 
 *
 * @ingroup serial
 */
class Print
{
  public:
    /** Define scale name for print/println methods */
    enum SCALE{
         BIN = 2,   /**< 2 */
         OCT = 8,   /**< 8 */
         DEC = 10,  /**< 10 */
         HEX = 16,  /**< 16 */
    };
  
    /**
     * Pure virtual method called by print/println methods. 
     * Subclass must implement it for using print/println methods with format output.
     * 
     * @param c - Data to write
     * @return The length of data written in byte. Acutally, 1 if success, 0 if fail. 
     */
    virtual size_t write(uint8_t c) = 0;

    /** 
     * Print data from buffer. 
     *
     * @param buffer - Pointer to data
     * @param size - The size of data to write
     * @return The legnth in byte of data written actually. 
     */
    size_t write(const uint8_t *buffer, size_t size) {
        size_t n = 0;
        while (size--) {
            n += write(*buffer++);
        }
        return n;
    }
  
    /**
     * Print a string
     *
     * @param str - Pointer to an array of type char
     * @return The legnth in byte of data written actually. 
     */
    size_t write(const char *str) {
      if (str == NULL) return 0;
      return write((const uint8_t *)str, strlen(str));
    }
    
    size_t print(const string & s);
    size_t print(const char str[]);
    size_t print(char c);
    size_t print(unsigned char b, int base = DEC);
    size_t print(int n, int base = DEC);
    size_t print(unsigned int n, int base = DEC);
    size_t print(long n, int base = DEC);
    size_t print(unsigned long n, int base = DEC);
    size_t print(double n, int digits = 2);

    size_t println(const string &s);
    size_t println(const char str[]);
    size_t println(char c);
    size_t println(unsigned char b, int base = DEC);
    size_t println(int num, int base = DEC);
    size_t println(unsigned int num, int base = DEC);
    size_t println(long num, int scale = DEC);
    size_t println(unsigned long num, int base = DEC);
    size_t println(double num, int digits = 2);
    size_t println(void);

private:
    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);

    
};

#endif /* #ifdef __cplusplus */
#endif
