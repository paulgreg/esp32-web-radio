# esp32-web-radio

An ESP32 web radio, inspired by [KitchenRadio](https://github.com/jeroenlukas/KitchenRadio). 🙏

## Hardware

 - ESP32-WROOM-32D/ESP32-WROOM-32U with Antenna
 - VS1003B VS1053 MP3 Module Development Board Onboard recording function
 - 2.4" 128x64 OLED I2C SSD1309 Display Module
 - 2 buttons
 

## Connections

  | ESP32  | VS1053 | NEXT BTN | PREV BTN | OLED |
  |--------|--------|----------|----------|------|
  |  GND   |  GND   |    GND   |   GND    | GND  |
  |  5V    |  5V    |          |          |      |
  |  3.3V  |  3.3V  |          |          |      |
  |  IO18  |  SCK   |          |          |      |
  |  IO19  |  MISO  |          |          |      |
  |  IO23  |  MOSI  |          |          |      |
  |  EN    |  XRST  |          |          |      |
  |  IO5   |  CS    |          |          |      |
  |  IO16  |  DCS   |          |          |      |
  |  IO4   |  DREQ  |          |          |      |
  |  IO32  |        |    X     |          |      |
  |  IO33  |        |          |     X    |      |
  |  IO21  |        |          |          | SDA  |
  |  IO22  |        |          |          | SCL  |


*Don’t use GND next to 5V ! Uploading will crash*

From [that example](https://github.com/baldram/ESP_VS1053_Library/blob/master/examples/WebRadioDemo/WebRadioDemo.ino).


## Configuration

Copy `parameters.h.dist` to `parameters.h` and change it to your settings

Note : some VS1053 board are’nt able to decode aac streams. You can usually find mp3 alternative (or change board).

## Librairies

- [CelliesProjects/ESP32_VS1053_Stream](https://github.com/CelliesProjects/ESP32_VS1053_Stream)
- [madleech/Button](https://github.com/madleech/Button)


## Flash

Flash using « ESP32 DEV Module »


## References

- [VS1053 Datasheet](https://www.sparkfun.com/datasheets/Components/SMD/vs1053.pdf)
- https://www.instructables.com/HiFi-Online-Radio-Internet-Streaming-With-ESP32-an/
- to generate font : https://oleddisplay.squix.ch/
