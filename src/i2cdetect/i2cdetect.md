
# I2CDetectGL

## Overview

Raspberry Pi I2C bus scan detection program.
Detects I2C 7-bit address and prints them to screen.
User-space program to scan for I2C devices, to see if any slaves are present on the I2C bus.
Uses bcm2835 Library v1.7.5 dependency. Provides low level I2C bus, delays and GPIO control.

## Dependency

Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.7.5)
Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

## Installation

Enter your Downloads folder, then type in terminal.
To download, build , install and test.

```sh
curl -L -O https://github.com/gavinlyonsrepo/RPI_Tools/archive/main.zip
unzip main.zip
cd RPI_Tools-main/src/i2cdetect
make
sudo make install
```

## Usage

To print help message

```sh
sudo i2cDetectGL -h
```

## Software

### Options

|  Flag | Description |
|  --- | ----|
| -h | print help message|
| -v | print version numbers |
| -m | Scan I2C bus and print table |
| -s | Scan I2C bus and print detected devices |
| -a I2C_ADDRESS | Probe a 7-bit I2C address passed as 2nd argument eg 0x3C |


### Errors

| Error number | Description | Solution |
| --- | ----| ----| 
| 1201 | Cannot open bcm2835 library | Run as sudo |
| 1202 | Cannot start I2C bus | run as sudo , enable I2c bus in rpi config |
| 1203 | Unsupported option | |
| 1204 | No i2c address specified for -a option | Enter an I2C Address eg i2cDetectGL -a 0x3C |
| 1205 | In valid I2C address passed for -a option| Enter a valid 7-bit I2c Address range 0x00 to 0x7F |
| 1206 | Reserved Address chosen for -a option | 0x00 to 0x07 and 0x78 to 0x7F are reserved |

## Output

Detecting an SSD1306 I2C OLED on i2C Bus at address  0x3C.

1. @ = Detection of device on bus.
2. . = No detection 
3. R = Reserved address 

![i2c](https://github.com/gavinlyonsrepo/RPI_TOOLS/blob/main/extras/image/i2c.png)
