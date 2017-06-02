2017 Propagand-Eye
=======================================
<p align="center">
<img src = "http://openponics.com/imgs/apoboard2017.jpg">
</p>

This board uses an Atmega328p microcontroller (the same device in the Arduino Uno) and can be programmed with an FTDI serial adapter and the Arduino IDE. There is a DC/DC boost-converter (the PAM2401) to provide 5 volts from a single AA battery. On board are 10 addressable WS2812 LEDs (the same LEDs used on all Neopixel products). There is also an infrared (IR) transmitter/receiver pair that allows the boards to communicate with one another. A tactile button is also provided to switch through modes.

Hacking Your Eyeball
=======================================
This section will cover all the information you'll need to start hacking your eye via hardware, software or both.

Arduino IDE Settings
----------------------
* Arduino IDE Version - 1.6.8
* Board - Arduino Uno
* Baud Rate - 115200

Hardware Pin connections
------------------
* Button Pin - D2
* WS2812 LED Pin - D7
* IR_RX Pin - D8
* IR_TX Pin - D9

LED Positioning 
----------------

```     5
     4     6
  3           7
  2           8
     1     9
        0```


Notes About the Board
---------------------
* The thru-hole WS2812 LEDs are GRB, not RGB
* The LEDs boot up in the obnoxious yet hilarious full-on Blue. There doesn't seem to be anything that can be done about it in software. 

Using with the IR Library
-----------------------

You can use this board with the [Arduino IR Library](https://github.com/z3t0/Arduino-IRremote/blob/master/boarddefs.h). However, you will need to change one line of code in the library. There is a file call [boarddefs.h](https://github.com/z3t0/Arduino-IRremote/blob/master/boarddefs.h). In it, look for this section:
```
  #else
  // Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, Nano, etc
  // ATmega48, ATmega88, ATmega168, ATmega328
	//#define IR_USE_TIMER1   // tx = pin 9
  #define IR_USE_TIMER2 // tx = pin 3
```
Uncomment the line that says `pin 9`, and comment out the line that says `pin 3`. It should now look like this:
```
  #else
  // Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, Nano, etc
  // ATmega48, ATmega88, ATmega168, ATmega328
	#define IR_USE_TIMER1   // tx = pin 9
  //#define IR_USE_TIMER2 // tx = pin 3
  ```
You should now be able to use the IR library with the Propagand-Eye. 

License Information
-------------------

All of these files are developed for free, for Apogaea, and for fun. It is publicly available under the GNU General Public License version 2 (the "GPL License"). For more information, please see: http://www.gnu.org/licenses/gpl-2.0.html

Copyright (c) John English, Joel Bartlett, 2017 Licensing: GPL v2
