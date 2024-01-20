# esp32-web-radio

An ESP32 web radio, inspired by [KitchenRadio](https://github.com/jeroenlukas/KitchenRadio). 🙏

## Hardware

 - ESP32-WROOM-32D/ESP32-WROOM-32U with Antenna
 - VS1003B VS1053 MP3 Module Development Board Onboard recording function

## Connections


  | VS1053  | ESP8266 |  ESP32   |
  ----------|---------|-----------
  |   SCK   |   D5    |   IO18   |
  |   MISO  |   D6    |   IO19   |
  |   MOSI  |   D7    |   IO23   |
  |   XRST  |   RST   |   EN     |
  |   CS    |   D1    |   IO5    |
  |   DCS   |   D0    |   IO16   |
  |   DREQ  |   D3    |   IO4    |
  |   5V    |   5V    |   5V     |
  |   GND   |   GND   |   GND    |

*Don’t use GND next to 5V ! It won’t boot in my case*

From [that example](https://github.com/baldram/ESP_VS1053_Library/blob/master/examples/WebRadioDemo/WebRadioDemo.ino).


## Configuration

Copy `parameters.h.dist` to `parameters.h` and change it to your settings


## Librairies

- [baldram/ESP_VS1053_Library](https://github.com/baldram/ESP_VS1053_Library/) : `cd Arduino/libraries && git clone https//github.com/baldram/ESP_VS1053_Library.git)`
- [CelliesProjects/ESP32_VS1053_Stream](https://github.com/CelliesProjects/ESP32_VS1053_Stream) : `cd Arduino/libraries && git clone https://github.com/CelliesProjects/ESP32_VS1053_Stream`

## Flash

Flash using « ESP32 DEV Module »

## References

- [VS1053 Datasheet](https://www.sparkfun.com/datasheets/Components/SMD/vs1053.pdf)
