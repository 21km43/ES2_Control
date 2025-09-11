# ES2 control tool

## Hardware

* [Arduino UNO R3](https://docs.arduino.cc/hardware/uno-rev3)

### Pins

* SDA, SCL ... to control DCDC converter and INA228
* GPIO 7 ... to control reset signal

## Software

* [PlatformIO](https://platformio.org)
* [VSCode](https://code.visualstudio.com)

### Library

* [Adafruit_INA228](https://github.com/adafruit/Adafruit_INA228)

If you want to develop on [Arduino IDE](https://www.arduino.cc/en/software), copy [main.cpp](/src/main.cpp) to .ino file, install libraries, and build - upload.
