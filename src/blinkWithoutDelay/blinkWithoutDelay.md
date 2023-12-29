
## Overview

Example code to replicate famous Arduino sketch blink without delay 
on a Raspberry Pi with C++.

* Development Tool chain. 
	1. Raspberry PI 3 model b
	2. C++, g++ (Debian 12.2.0) 
	3. Raspbian , Debian 12 bookworm OS, , 64 bit.
	3. kernel : aarch64 Linux 6.1.0-rpi7-rpi-v8


## Optional Dependency

Uses bcm2835 Library v1.73 dependency. Provides GPIO control.
Output is to console and LED, if user does not want LED output they can remove all references to bcm2835 library and LED from code.
bcm2835 library is only required to control LED GPIO.

1. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.73.)
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

## Setup and test

Download to Downloads folder, Enter 'src/blinkWithoutDelay' folder then type in terminal.
to compile and test.

```sh
make
./bin/blinkWithoutDelay.out n
```

Command line argument

| Number | Format | Function |
| --- | --- | --- |
| 1 | n | Nanosecond timing resolution calculation | 
| 1 | r | Microsecond timing resolution calculation |
| 1 | m | Millisecond timing resolution calculation |

## Output 

Output is blinking LED and Text to console.
 
![blink](https://github.com/gavinlyonsrepo/RPI_TOOLS/blob/main/extras/image/blink.png)
