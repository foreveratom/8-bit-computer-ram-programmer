# 8-bit-computer-ram-programmer
[Bean Eater's 8 Bit Computer](https://eater.net/8bit) - RAM module programmer over WiFi / HTTP


This project uses an [ESP8266](https://en.wikipedia.org/wiki/ESP8266) to replace the [RAM programming module](https://eater.net/8bit/ram) dip switches and push buttons for the 8-Bit Computer. The ESP8266 hosts a HTTP server from which programs can be loaded into the computer. 

## Seriously, why?

One may argue that adding a super-capable microcontroller to the 8-bit computer totally defeats the purpose. Those people are right, overkill is the definition of this project.

However, too much sanity was lost trying to deal with the dip switches I have and their flaky connections to breadboards and a different solution was needed, badly. Also how cool would 'that' be to be able to program the computer from anywhere?

I had an ESP12 board lying around from past tinkering; this thing has wireless and bluetooth built-in, acts and programs like an Arduino; it's wonderful and totally suited for the job. 

I had the technology...you know the drill.

## Disclaimer

I am an electronic beginner and learned most of it while building [Ben Eater's 8 Bit Computer](https://eater.net/8bit), which is a wonderful video series. That is to say I don't exactly know what I'm doing besides what I remember from school: `U = RI`. While I am comfortable with programming, the source code is in C++ which I had not visited since the advent of Java.

You will most likely see mistakes, horrors and other stranger things in the wiring and/or code. Don't kill me if your computer ends up in smoke but please make sure to let me know of any error, improvement or else, and don't hesitate to educate me.

Redditor /u/foreveratom

## Build 

The module follows the same basics as the [EEPROM programmer](https://www.youtube.com/watch?v=K88pgWhEb1M) in that it uses two 74HC595 shift registers to provide more output lines (16 in total) than what [ESP8266 boards can provide](https://github.com/thehookup/Wireless_MQTT_Doorbell/blob/master/GPIO_Limitations_ESP8266_NodeMCU.jpg). The build uses all 'safely' available pins (D1, D2, D5, D6, D7 and D8) with 3 pins (D5, D7 and D8) used for SPI transfer with the registers.

To build the module you will need the room on roughly half a breadboard and two 74HC595 shift registers in addition to the ESP8266. You should have enough room on your 8-bit computer if you remove the dip switches and the logic components around the existing RAM programming module.

Follow the [wiring in this article](https://techtutorialsx.com/2016/09/04/esp8266-controlling-chained-sn74hc595-ics/) to wire the ESP8266 to the shift registers; the code currently uses the following mapping:

```
D1 -> Halt
D2 -> Program Mode
D5 -> SPI clock (SCLK)
D7 -> SPI data (MOSI)
D8 -> SPI Master/Slave (SS)
```

With the current code's pin mapping, the shit register (A) receiving the data signal (SER) outputs memory register addresses and a halt (HLT), program mode (PRG) and reset (RST) flags - the memory register outputs should replace the memory register dip switches from the computer and the flag outputs should be wired to the clock HLT, program mode switch and reset switch respectively.

##### Shit Register A
```
QA -> unused (reserved for 'activate laser' function)
QB -> Reset flag
QC -> Program mode flag
QD -> Halt flag

QE -> Memory Register bit 1 
QF -> Memory Register bit 2
QG -> Memory Register bit 3
QH -> Memory Register bit 4
```

The second register (B) outputs the 8-bit of memory content for the memory register stored in the first register.

##### Shit Register B
```
QA -> Memory bit 1
QB -> Memory bit 2
QC -> Memory bit 3
QD -> Memory bit 4
QE -> Memory bit 5
QF -> Memory bit 6
QG -> Memory bit 7
QH -> Memory bit 8
```

##### Tip

While testing your module with LEDs (of course you do, right?), make sure you put a proper resistor from the LED to ground (I use 420 Ohms resistors). The output of ESP8266 is enough to fry most of the LEDs or make you see stars for a while if you stare at them for too long.

##### About using HC vs LS 74xxx

Because I could not find any 74LS595 easily, following the EEPROM programmer this build uses their HC variant. It is usually recommended not to mix LS and HC components together but while using HC inputs from LS variants is incorrect, it is safe to use HC as output only to LS chips. At least the internet says so.

## Documentation

### Quick Start

* Follow the numerous articles on the web to set your board up and running with Arduino IDE or else.

* Edit the main sketch (.ino) to adjust for your wireless network.

* Open a Serial Monitor console window for server output.

* Run the sketch: this will start a HTTP server and bind to an address provided by your network. The IP address of the server will be 
shown in the console.

* Point your browser to `http://ip-of-your-thing/`. You should see a pre-loaded program ready to be written.

### HTTP Commands

The HTTP server allows a few commands to be sent to it through a GET request (parameterized URL); note that nothing is sent to the computer until a `write` command is issued.

##### 'clear' 

`http://8bit/?clear`

This will erase the program loaded on the server.

##### 'load' 

`http://8bit/?load=<PROGRAM>`

This will load the provided program on the server. The program should be expressed as a semi-colon-separated-value string containing each instruction, up to 16.

e.g: the following program

```
LDA 15
OUT
HLT
```

should be sent as `http://8bit/?load=LDA 15; OUT; HLT`

##### 'write'

`http://8bit/?write`

`http://8bit/?write&interval=1000`

This will write the loaded program to the computer. You can specify an optional interval in milliseconds (minimum 100) between each writing step which can be useful for debugging or because it looks pretty at slow speed with all those LEDs blinking.

#### Chaining commands

You can chain the above commands to instruct the server to do more than one thing. The following example clears the current program, loads a new one and writes it to the computer.

`http://8bit/?clear&load=LDA 15; OUT; HLT&write`

Note that the order of parameters does not matter. *clear* is always executed first, then *load*, then *write*.

## Links

* I could not have started this without Ben Eater's explanation of shit registers while building the [EEPROM programmer module](https://www.youtube.com/watch?v=K88pgWhEb1M).

* This article about [controlling chained 74HC595 with I2C](https://techtutorialsx.com/2016/09/04/esp8266-controlling-chained-sn74hc595-ics/) explains how to chain shift registers and use SPI to push data into them.

* [Pin mapping of the ESP8266](http://esp8266.github.io/Arduino/versions/2.0.0/doc/reference.html). Be aware that your experience in pin mapping may vary depending on the development board used. The current source is [mapped to a NodeMCU](https://techtutorialsx.com/2017/04/02/esp8266-nodemcu-pin-mappings/) board.

* [SN74HC595 datasheet](http://www.ti.com/lit/ds/symlink/sn74hc595.pdf).
