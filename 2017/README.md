2017 Propagand-Eye
=======================================
<p align="center">
<img src = "http://openponics.com/imgs/apoboard2017.jpg">
</p>

This board uses an Atmega328p microcontroller (the same device in the Arduino Uno) and can be programmed with an FTDI serial adapter and the Arduino IDE. There is a DC/DC boost-converter (the PAM2401) to provide 5 volts from a single AA battery. On board are 10 addressable WS2812 LEDs (the same LEDs used on all Neopixel products). There is also an infrared (IR) transmitter/receiver pair that allows the boards to communicate with one another. A tactile button is also provided to switch through modes.

Read more about the project [here](https://www.sparkfun.com/news/2398). 

Hacking Your Eyeball
=======================================
This section will cover all the information you'll need to start hacking your eyeball.

<p align="center">
<img src = "https://cdn.sparkfun.com/r/500-500/assets/home_page_posts/2/3/9/8/Propagand-eye-01.jpg">
</p>

<p align="center">
<img src = "https://cdn.sparkfun.com/r/500-500/assets/home_page_posts/2/3/9/8/Propagand-eye-02.jpg">
</p>

FTDI Headers
--------------------
Before you can program your Propagand-Eye, you'll need to solder either some [right-angle headers](https://www.sparkfun.com/products/553) or [straight headers](https://www.sparkfun.com/products/116) to the FTDI port on your eyeball. 

<p align="center">
<img src = "https://cdn.sparkfun.com/r/500-500/assets/home_page_posts/2/3/9/8/FTDI.png">
</p>

Then, you'll need a [5V FTDI Basic](https://www.sparkfun.com/products/9716) or similar 5V Serial-to-USB converter to program your eyeball with the [Arduino IDE](https://www.arduino.cc/en/Main/Software). 

Arduino IDE Settings
----------------------
* Arduino IDE Version - 1.6.8 (This code will only compile in 1.6.~ versions of Arduino, not the latest versions)
* Board - Arduino Uno
* Baud Rate - 115200

Hardware Pin connections
------------------
* Button Pin - D2
* WS2812 (Neopixel) LED Pin - D7
* IR_RX Pin - D8
* IR_TX Pin - D9

LED Positioning 
----------------
```
        5
     4     6
  3           7
  2           8
     1     9
        0
```

A4 & A5/I2C Breakouts
-------------------
On the backside of the board you'll find four test points: <b>GND, A4(SDA), A5(SCL), and 5V</b>. You can solder a number of accessory sensors and parts to these pins to enhance your eyeball. For instance, you could add a [Real-Time Clock module](https://www.sparkfun.com/products/12708), and give your eyeball the ability to know what time it is, even if it looses power. Any 5V i2C or analog sensor can be added. The analog pins can also be used as digital pins to control other electronics. 

<p align="center">
<img src = "https://cdn.sparkfun.com/assets/home_page_posts/2/3/9/8/i2c.png">
</p>


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
