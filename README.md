Software
========

This Repo contains source code for the DIY Interactive Electronics Apogaea PCB.

The system is based on an Atmega328p microcontroller (the same device in the Arduino UNO), and can be programmed with an [FTDI serial adapter](https://www.sparkfun.com/products/9716) and the [Arduino IDE](https://arduino.cc).

2013-14 Sound-reactive Sheriff Star
=======================================
<p align="center">
<img src = "http://openponics.com/imgs/apoboard2013-14.jpg">
</p>

The software reads analog signals from the on-board microphone amplifier and performs an FHT (Fast Hartley Transform), which outputs an array of frequency values. We use this array to decide how to control the LEDs, and repeat the process. The Software also support multiple modes, which the user can switch between with the on-board momentary tactile button. This software is alpha-quality, and may or may not work as expected!


The system is based on an Atmega328p microcontroller (the same device in the Arduino Uno) and can be programmed with an FTDI serial adapter and the Arduino IDE. There is a DC/DC boost-converter to provide 5 volts from a single AA battery to a microphone, amplifier, and 3 RGB LEDs that are driven from common PWM channels. There is also a momentary tactile button for user input.

2017 Propagand-Eye
=======================================
<p align="center">
<img src = "http://openponics.com/imgs/apoboard2017.jpg">
</p>

The Propagand-Eye is an eyeball-shaped PCB onto which participants will solder addressable LEDs as well as a variety of other components to make their own blinky badge they can wear with pride. Once assembled, the badges will be able to communicate with one another via infrared (IR) communication, creating an interactive game in which participants will be able to collect different blinky modes from other badges. This will create many opportunities for interaction among participants long after the workshop is over.

This board uses an Atmega328p microcontroller (the same device in the Arduino Uno) and can be programmed with an FTDI serial adapter and the Arduino IDE. There is a DC/DC boost-converter (the PAM2401) to provide 5 volts from a single AA battery. On board are 10 addressable WS2812 LEDs (the same LEDs used on all Neopixel products). There is also an infrared (IR) transmitter/receiver pair that allows the boards to communicate with one another. A tactile button is also provided to switch through modes.


License Information
-------------------

All of this code is developed for free, for Apogaea, and for fun. It is publicly available under the GNU General Public License version 2 (the "GPL License"). For more information, please see: http://www.gnu.org/licenses/gpl-2.0.html

Copyright (c) John English, Joel Bartlett, 2017 Licensing: GPL v2
