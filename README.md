# Breadbot

Breadbot is an Arduino Uno powered bread proofer and data logger.

## Overview

- It uses a [thermistor to sense the temperature][1].
- [When the temperature is below 27°C (80°F), a relay connects power from the wall outlet][3] to a [clamp light][clamplight] which is fitted with an incandescent globe. This provides enough heat keep the inside of an oven at 27°C.
- One [button cycles through the stages of making bread][5]: Pre-Ferment, Autolyse, Bulk Ferment, Bench Rest, Proofing
- The other button starts and stops a timer. [And this is all logged to an SD card as a CSV file.][4]
- [A 2x16 LCD screen][2] shows the current temperature, the target temperature, breadmaking stage and time if logging is on.

## Parts

- [This starter kit][kit] includes an Arduino Uno, breadboard, thermistor, LCD screen, plus a whole bunch of wires, resistors etc. that you might need.
- A [clamp light][clamplight] is a cheap lamp to use but I want to try using this [heating element][heatingelement] as it's 200W and would probably do just as well as an incandescent globe with less bulk.
- You need a [Micro SD Card Module][microsdmodule] that does level shifting. The first one I bought didn't do this and I fried a card and had a few hours of frustration. Get a [lower capacity Micro SD card][microsdcard] (I think 32gb is the max). This needs to be formatted in either FAT16 or FAT32. It's recommended that you do it with the [SD Association SD Card Formatter][formatter].
- I put it all inside a [Pelican Case][pelicancase] which I drilled holes in for power access and for the SD Card Module and buttons to be mounted on the outside of the case.
- I think I'll replace the buttons I have with [these more robust buttons][buttons]. I don't want the wires to disconnect in 6 months and have to work out how to wire it again.


[1]: http://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
[2]: http://www.circuitbasics.com/how-to-set-up-an-lcd-display-on-an-arduino/
[3]: http://www.circuitbasics.com/setting-up-a-5v-relay-on-the-arduino/
[4]: http://www.geeetech.com/wiki/index.php/Arduino_SD_card_Module
[5]: http://www.instructables.com/id/Arduino-Button-Tutorial/
[kit]: http://a.co/i5MgfrV[relay]: http://a.co/bRE1c1w
[clamplight]: http://a.co/6ZlEt3F
[heatingelement]: http://a.co/fAGOS5C
[microsdcard]: http://a.co/8nRd3xS
[microsdmodule]: http://a.co/fafyDOt
[pelicancase]: http://a.co/2W9eEEu
[formatter]: https://www.sdcard.org/downloads/formatter_4/
[buttons]: http://a.co/3CJpNLx
