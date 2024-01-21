# esp32-web-radio

An ESP32 web radio, inspired by [KitchenRadio](https://github.com/jeroenlukas/KitchenRadio). 🙏

## Hardware

 - ESP32-WROOM-32D/ESP32-WROOM-32U with Antenna
 - VS1003B VS1053 MP3 Module Development Board Onboard recording function
 - 2 buttons
 - SPI 240x320 TFT screen


## Connections

  | ESP32  | VS1053 | NEXT BTN | PREV BTN |
  |--------|--------|----------|----------|
  |  GND   |  GND   |    GND   |    GND   |
  |  5V    |  5V    |          |          |
  |  IO18  |  SCK   |          |          |
  |  IO19  |  MISO  |          |          |
  |  IO23  |  MOSI  |          |          |
  |  EN    |  XRST  |          |          |
  |  IO5   |  CS    |          |          |
  |  IO16  |  DCS   |          |          |
  |  IO4   |  DREQ  |          |          |
  |  IO32  |        |    X     |          |
  |  IO33  |        |          |     X    |

*Don’t use GND next to 5V ! Uploading will crash*

From [that example](https://github.com/baldram/ESP_VS1053_Library/blob/master/examples/WebRadioDemo/WebRadioDemo.ino).


## Configuration

Copy `parameters.h.dist` to `parameters.h` and change it to your settings


## Librairies

- [CelliesProjects/ESP32_VS1053_Stream](https://github.com/CelliesProjects/ESP32_VS1053_Stream) : `cd Arduino/libraries && git clone https://github.com/CelliesProjects/ESP32_VS1053_Stream`


## Flash

Flash using « ESP32 DEV Module »


## References

- [VS1053 Datasheet](https://www.sparkfun.com/datasheets/Components/SMD/vs1053.pdf)
- https://www.instructables.com/HiFi-Online-Radio-Internet-Streaming-With-ESP32-an/
