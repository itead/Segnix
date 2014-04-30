/*
LCD4Bit v0.1 16/Oct/2006 neillzero http://abstractplain.net

What is this?
An arduino library for comms with HD44780-compatible LCD, in 4-bit mode (saves pins)

Sources:
- The original "LiquidCrystal" 8-bit library and tutorial
    http://www.arduino.cc/en/uploads/Tutorial/LiquidCrystal.zip
    http://www.arduino.cc/en/Tutorial/LCDLibrary
- DEM 16216 datasheet http://www.maplin.co.uk/Media/PDFs/N27AZ.pdf
- Massimo's suggested 4-bit code (I took initialization from here) http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1144924220/8
See also:
- glasspusher's code (probably more correct): http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1160586800/0#0

Tested only with a DEM 16216 (maplin "N27AZ" - http://www.maplin.co.uk/Search.aspx?criteria=N27AZ)
If you use this successfully, consider feeding back to the arduino wiki with a note of which LCD it worked on.

Usage:
see the examples folder of this library distribution.

MODIFIED BY !
Desc: This lib is transplanted to itead sdk by Wu Pengfei.
Date: 2014.3.31
Email: pengfei.wu@itead.cc

*/

#ifndef itead_LCD4Bit_mod_h
#define itead_LCD4Bit_mod_h

class LCD4Bit_mod {
public:
  LCD4Bit_mod(int num_lines);
  void commandWrite(int value);
  void init();
  void print(int value);
  void printIn(char value[]);
  void clear();
  //non-core---------------
  void cursorTo(int line_num, int x);
  void leftScroll(int chars, int delay_time);
  //end of non-core--------

  //4bit only, therefore ideally private but may be needed by user
  void commandWriteNibble(int nibble);
private:
  void pulseEnablePin();
  void pushNibble(int nibble);
  void pushByte(int value);
};

#endif
