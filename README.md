# arduino_display

## Description

A sketch for a small Arduino system (Duemilanove ATmega328) with a WiShield (1.0) and a basic serial enabled 16x2 LCD from [SparkFun](https://www.sparkfun.com/products/9069).

The Arduino program fetches data every 10 minutes from a web site and displays the data using the LCD.

## Details

- I use a Ruby CGI script (`info.cgi`) installed on a web server to do the heavy duty HTML parsing. The CGI script gives the simplified info text.
- The WiShield does not support DNS. Thus, you can't use Heroku or other cloud based hosts. The web server needs to be accessible using an IP address.
- You can do the HTML parsing on Arduino provided that the web site is small enough for the memory.

## Build

You need to take a look at the source and configure it accordingly.

How to build and deploy the sketch. First, install [Ino](http://inotool.org) and then:

	$ ino build -m atmega328
	$ ino upload -m atmega328 -p /dev/ttyUSB0
	$ ino serial -b 57600 -p /dev/ttyUSB0 # To see the serial output.

## Colophon

I got an Arduino fever the other week. So, I resurrected my old Arduino and did a bit of hacking over the weekend.

Take a look at the photos :)

[Photo1](https://raw.github.com/donny/arduino_display/master/photos/photo1.jpg)
[Photo2](https://raw.github.com/donny/arduino_display/master/photos/photo2.jpg)
[Photo3](https://raw.github.com/donny/arduino_display/master/photos/photo3.jpg)
