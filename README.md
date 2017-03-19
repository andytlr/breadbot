# Breadbot

Breadbot is an Arduino Uno powered bread proofer and data logger.

## Overview

- It uses a [thermistor to sense the temperature][1].
- [When the temperature is below 27°C (80°F), a relay connects power from the wall outlet][3] to a [clamp light][7] which is fitted with an incandescent globe. This provides enough heat keep the inside of an oven at 27°C.
- One [button cycles through the stages of making bread][5]: Pre-Ferment, Autolyse, Bulk Ferment, Bench Rest, Proofing
- The other button starts and stops a timer. [And this is all logged to an SD card as a CSV file.][4]
- [A 2x16 LCD screen][2] shows the current temperature, the target temperature, breadmaking stage and time if logging is on.

## Parts

- [This starter kit][6] includes an Arduino Uno, breadboard, thermistor, LCD screen, plus a whole bunch of wires, resistors etc. that you might need.
- A [clamp light][7] is a cheap lamp to use but I want to try using this [heating element][8] as it's 200W and would probably do just as well as an incandescent globe with less bulk.
- The relay that comes in the kit isn't mounted on the board and I had all kinds of trouble with it. So I got [this one][14].
- You need a [Micro SD Card Module][10] that does level shifting. The first one I bought didn't do this and I fried a card and had a few hours of frustration. Get a [lower capacity Micro SD card][9] (I think 32gb is the max). This needs to be formatted in either FAT16 or FAT32. It's recommended that you do it with the [SD Association SD Card Formatter][12].
- I put it all inside a [Pelican Case][11] which I drilled holes in for power access and for the SD Card Module and buttons to be mounted on the outside of the case.
- I think I'll replace the buttons I have with [these more robust buttons][13]. I don't want the wires to disconnect in 6 months and have to work out how to wire it again.


[1]: http://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
[2]: http://www.circuitbasics.com/how-to-set-up-an-lcd-display-on-an-arduino/
[3]: http://www.circuitbasics.com/setting-up-a-5v-relay-on-the-arduino/
[4]: http://www.geeetech.com/wiki/index.php/Arduino_SD_card_Module
[5]: http://www.instructables.com/id/Arduino-Button-Tutorial/
[6]: http://a.co/i5MgfrV
[7]: http://a.co/6ZlEt3F
[8]: http://a.co/fAGOS5C
[9]: http://a.co/8nRd3xS
[10]: http://a.co/fafyDOt
[11]: http://a.co/2W9eEEu
[12]: https://www.sdcard.org/downloads/formatter_4/
[13]: http://a.co/3CJpNLx
[14]: http://a.co/bRE1c1w
