This is a C++ library for Arduino for controlling an HD74800-compatible LCD in 4-bit mode.
Tested on Arduino 0010 

Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

When installed, this library should look like:

arduino-0010/hardware/libraries/LCD4Bit_mod              (this library's folder)
arduino-0010/hardware/libraries/LCD4Bit_mod/LCD4Bit_mod.cpp  (the library implementation file)
arduino-0010/hardware/libraries/LCD4Bit_mod/LCD4Bit_mod.h    (the library description file)
arduino-0010/hardware/libraries/LCD4Bit_mod/keywords.txt (the syntax coloring file)
arduino-0010/hardware/libraries/LCD4Bit_mod/examples     (the examples in the "open" menu)
arduino-0010/hardware/libraries/LCD4Bit_mod/readme.txt   (this file)

Building
--------------------------------------------------------------------------------

After this library is installed, you just have to start the Arduino application.
You may see a few warning messages as it's built.

To use this library in a sketch, go to the Sketch | Import Library menu and
select LCD4Bit_mod.  This will add a corresponding line to the top of your sketch:
#include <LCD4Bit_mod.h>

To stop using this library, delete that line from your sketch.

Geeky information:
After a successful build of this library, a new file named "LCD4Bit_mod.o" will appear
in "Arduino/lib/targets/libraries/LCD4Bit". This file is the built/compiled library
code.

If you choose to modify the code for this library (i.e. "LCD4Bit_mod.cpp" or "LCD4Bit_mod.h"),
then you must first 'unbuild' this library by deleting the "LCD4Bit_mod.o" file. The
new "LCD4Bit_mod.o" with your code will appear after the next press of "verify"

MODIFIED BY !
Desc: This lib is transplanted to itead sdk by Wu Pengfei.
Date: 2014.3.31
Email: pengfei.wu@itead.cc

