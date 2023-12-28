

## Overview
Raspberry Pi I2C bus scan detection program
Detects I2C 7-bit address and prints them to screen using bcm2835 library.
Uses bcm2835 Library v1.73 dependency. Provides low level I2C bus, delays and GPIO control.

## Dependency

1. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.73.)
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

## Setup

Download to Downloads folder, Enter 'src/i2cdetect' folder then type in terminal.
to compile and test.

```sh
make
make run
```

In ./bin/ folder there will be a executable file called i2cDetectGL.
Copy this to a folder where you can run it eg (/usr/sbin). Note bcm2835 library 
requires sudo.

```sh
sudo i2cDetectGL
```

## Output 

![i2c](https://github.com/gavinlyonsrepo/RPI_TOOLS/blob/main/extras/image/i2c.png)
