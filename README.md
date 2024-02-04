# esp32-web-radio

An ESP32 web radio, inspired by [KitchenRadio](https://github.com/jeroenlukas/KitchenRadio). 🙏

## Features

 - mp3 and aac decoding (depends of VS1053 module)
 - OLED screen displaying radio, songs, volume
 - fetch radios from a json file (to easily update them)
 - IR remote (next / previous / volume + / volume - / mute)
 - next / previous button


## Hardware

 - ESP32-WROOM-32D/ESP32-WROOM-32U with Antenna
 - VS1003B VS1053 MP3 Module Development Board (make sure it decodes AAC for better stream quality)
 - 2.4" 128x64 OLED I2C SSD1309 Display Module
 - 2 buttons
 - IR receiver
 - IR remote (from an old DVD player)
 

## Connections

  | ESP32  | VS1053 | NEXT BTN | PREV BTN | OLED | IR |
  |--------|--------|----------|----------|------|----|
  |  GND   |   X    |     X    |    X     |  X   | G  |
  |  5V    |  5V    |          |          |      | R  |
  |  3.3V  |  3.3V  |          |          |      |    |
  |  IO18  |  SCK   |          |          |      |    |
  |  IO19  |  MISO  |          |          |      |    |
  |  IO23  |  MOSI  |          |          |      |    |
  |  EN    |  XRST  |          |          |      |    |
  |  IO5   |  CS    |          |          |      |    |
  |  IO16  |  DCS   |          |          |      |    |
  |  IO4   |  DREQ  |          |          |      |    |
  |  IO32  |        |    X     |          |      |    |
  |  IO33  |        |          |     X    |      |    |
  |  IO21  |        |          |          | SDA  |    |
  |  IO22  |        |          |          | SCL  |    |
  |  IO27  |        |          |          |      | Y  |


*Don’t use GND next to 5V ! Uploading will crash*

From [that example](https://github.com/baldram/ESP_VS1053_Library/blob/master/examples/WebRadioDemo/WebRadioDemo.ino).


## Configuration

Copy `parameters.h.dist` to `parameters.h` and change it to your settings.

⚠️ Web Radios are fetch from a JSON file. You'll need to create a JSON file containing web radios name and URL (see `WebRadios.h`) and host it somewhere on the internet.

Note : some VS1053 board aren't able to decode aac streams. You can usually find mp3 alternative (or change board).


## Librairies

- [CelliesProjects/ESP32_VS1053_Stream](https://github.com/CelliesProjects/ESP32_VS1053_Stream)
- [madleech/Button](https://github.com/madleech/Button)
- IRRemote


## Flash

Flash using « ESP32 DEV Module »


## References

- [VS1053 Datasheet](https://www.sparkfun.com/datasheets/Components/SMD/vs1053.pdf)
- https://www.instructables.com/HiFi-Online-Radio-Internet-Streaming-With-ESP32-an/
- to generate font : https://oleddisplay.squix.ch/
- https://www.makerguides.com/ir-receiver-remote-arduino-tutorial/
