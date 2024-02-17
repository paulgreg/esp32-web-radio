#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "font-roboto.h"
#define FONT_SMALL  &Roboto_Medium_12
#define FONT_MEDIUM &Roboto_Medium_14
#define FONT_BIG    &Roboto_Medium_16

// #include "font-dejavu.h"
// #define FONT_SMALL  &DejaVu_LGC_Sans_Mono_12
// #define FONT_MEDIUM &DejaVu_LGC_Sans_Mono_14
// #define FONT_BIG    &DejaVu_LGC_Sans_Mono_16

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupScreen() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setRotation(2);
  display.cp437(true);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void displayText(const char* s) {
  Serial.println(s);
  display.clearDisplay();
  display.setFont(FONT_BIG);
  display.setCursor(10, 40);
  display.print(s);
  display.display();
}

void displayError(const char* s) {
  Serial.println(s);
  display.clearDisplay();
  display.invertDisplay(true);
  display.setFont(FONT_BIG);
  display.setCursor(10, 40);
  display.print(s);
  display.display();
}

char radioBuffer[255];
char radioSong[255];

void displayData(const char* radio, const char* song, unsigned int volume, bool mute, bool eof) {
  Serial.printf("radio « %s », song « %s », vol: %i, mute: %i, eof: %i\n", radio, song, volume, mute, eof);

  formatString(radio, radioBuffer, RADIO_LEN_LIMIT);
  formatString(song, radioSong, SONG_LEN_LIMIT);

  display.clearDisplay();
  if (eof) { // End of stream
    display.setFont(FONT_MEDIUM);
    display.setCursor(0, 15);
    display.print(radioBuffer);
    display.setFont();
    display.setCursor(0, 35);
    display.print("Stream error");
  } else if (strlen(song) == 0) { // only radio name
    display.setFont(FONT_BIG);
    display.setCursor(0, 25);
    display.print(radioBuffer);
  } else { // with radio and song
    display.setFont();
    display.setFont(FONT_MEDIUM);
    display.setCursor(0, 12);
    display.print(radioBuffer);
    display.setFont(FONT_SMALL);
    display.setCursor(0, 32);
    display.print(radioSong);
  }

  display.setFont(FONT_SMALL);
  display.setCursor(94, 62);
  if (mute) {
    display.printf("  - %%");
  } else {
    display.printf("%*i %%", 3, volume);
  }

  display.display();
}
