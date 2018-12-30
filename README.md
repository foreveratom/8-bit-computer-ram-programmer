# 8-bit-computer-ram-programmer
[Bean Eater's 8 Bit Computer](https://eater.net/8bit) - RAM module programmer over WiFi / HTTP


This project uses an [ESP8266](https://en.wikipedia.org/wiki/ESP8266) to replace the [RAM programming module](https://eater.net/8bit/ram) dip switches and push buttons for the 8-Bit Computer. The ESP8266 hosts a HTTP server from which programs can be loaded into the computer. 

## Seriously, why?

One may argue that adding a super-capable microcontroller to the 8-bit computer totally defeats the purpose. Those people are right, overkill is the definition of this project.

However, too much sanity was lost trying to deal with the dip switches I have and their flaky connections to breadboards and a different solution was needed, badly. Also how cool would 'that' be to be able to program the computer from anywhere?

I had an ESP12 board lying around from past tinkering; this thing has wireless and bluetooth built-in, acts and programs like an Arduino; it's wonderful and totally suited for the job. 

I had the technology...you know the drill.


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

* This article about [controlling chained 74HC595 with I2C](https://techtutorialsx.com/2016/09/04/esp8266-controlling-chained-sn74hc595-ics/) explains how to chain shift registers and use I2C to push data into them.

* [Pin mapping of the ESP8266](http://esp8266.github.io/Arduino/versions/2.0.0/doc/reference.html). Be aware that your experience in pin mapping may vary depending on the development board used. The current source is [mapped to a NodeMCU](https://techtutorialsx.com/2017/04/02/esp8266-nodemcu-pin-mappings/) board.

