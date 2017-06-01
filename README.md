# hack_chime
## Hackintosh startup chime with Arduino

Detect BIOS beeps with Arduino and play the Mac boot chime

## Circuit

![Hack Chime circuit](https://github.com/mtgrosser/hack_chime/raw/master/circuit.png)

## BOM

* 1x Arduino Nano
* 1x DFPlayer Mini module with micro SD card
* 1x AMS1117 3.3 V voltage regulator module
* 4x BC337-16 or similar NPN transistor
* 1x 1 kΩ resistor
* 4x 2.7 kΩ resistor
* 2x 3 kΩ resistor
* 1x 10 Ω trimmer
* 1x 2-pin male header
* 1x 4-pin male header
* 1x speaker
* 1x ATX speaker cable

## PCB

![Hack Chime PCB](https://github.com/mtgrosser/hack_chime/raw/master/pcb.png)

## Device in action

![Hack Chime Device](https://github.com/mtgrosser/hack_chime/raw/master/device.jpg)

## Setup

The connector pins of the AMS1117 voltage regulator module must be re-soldered to
the opposite side of the module in order to fit the PCB. Please double-check VIN
and VOUT are wired correctly.

Pin 1 of the 4-pin header J2 must be connected to the "SPK" line of the BIOS speaker,
pin 4 must be connected to the "+" line. 

⚠️ **Pay attention not to accidentally swap the pins!**

The Arduino should be pre-flashed with the sketch before inserting it into the circuit.

#### Preparing the microSD card for DFPlayer

DFPlayer will sort the sound files in the order they are copied to the card.
Format the card and copy the files one by one. The order must match the sound/pin indexes
defined in the sketch, i.e. `chime`, `alert`, `funk`, `boink`. 
On macOS, make sure no "._" files end up on the card.

The output volume can be reduced using the R7 trimmer.

## Software

Depending on how much time the mainboard usually takes to complete its power-on self test (POST),
the variables const POST_WAIT_TIME and POST_TIMEOUT can be tweaked to catch the beeps.

## License

![Creative Commons License](https://i.creativecommons.org/l/by-sa/4.0/88x31.png) This work is licensed under a [Creative Commons Attribution-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-sa/4.0/).

