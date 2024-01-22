#include <Adafruit_GFX.h>       // include Adafruit graphics library
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, SPI_MOSI_PIN, SPI_CLK_PIN, TFT_RST, SPI_MISO_PIN);
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
#define BACKGND     ILI9341_BLACK 
#define COLOR_TEXT  ILI9341_WHITE
#define COLOR_ERROR ILI9341_RED

#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>

#define SCREEN_HORIZONTAL 1
#define SCREEN_HORIZONTAL_INVERSE 3

#define SCREEN_ORIENTATION SCREEN_HORIZONTAL

void screenDiagnostics() {
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("\nDisplay Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x\n\n"); Serial.println(x, HEX); 
}

void _text(int x, int y, int color, char* text) {
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.println(text);
}

void drawSmallText(int x, int y, char* text) {
  tft.setFont(&FreeMonoBold9pt7b);
  _text(x, y, COLOR_TEXT, text);
}

void drawBigText(int x, int y, char* text) {
  tft.setFont(&FreeMonoBold18pt7b);
  _text(x, y, COLOR_TEXT, text);
}

void drawText(int x, int y, char* text) {
  tft.setFont(&FreeMonoBold12pt7b);
  _text(x, y, COLOR_TEXT, text);
}

void printMsg(char* text) {
  tft.fillScreen(BACKGND);
  drawText(5, 50, text);
}

void printError(char* text) {
  tft.fillScreen(BACKGND);
  tft.setFont(&FreeMonoBold12pt7b);
  _text(0, 0, COLOR_ERROR, text);
}

void setupScreen() {
  tft.begin();
  screenDiagnostics();
  tft.setRotation(SCREEN_ORIENTATION);
  tft.fillScreen(BACKGND);
  tft.setTextSize(1);
  drawBigText(50, 140, "Web Radio");
}