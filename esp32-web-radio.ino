#include "parameters.h"

#include <Button.h>

#include <VS1053.h>
#include <ESP32_VS1053_Stream.h>

#include <Preferences.h>

#include <WiFi.h>
#include "network.h"

Preferences preferences;
ESP32_VS1053_Stream stream;

Button buttonNext(BTN_NEXT);
Button buttonPrevious(BTN_PREVIOUS);

unsigned short radioIdx = 0;
unsigned long lastRadioChange = millis();
bool radioIdxSaved = true;

void setup() {
  Serial.begin(115200);
  Serial.println("Web Radio");

  buttonNext.begin();
  buttonPrevious.begin();

  preferences.begin("webradio", false);
  radioIdx = preferences.getInt("radioIdx", radioIdx);

  delay(3000); // Wait for VS1053 and PAM8403 to power up
  SPI.setHwCs(true);
  SPI.begin(SPI_CLK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);  /* start SPI before starting decoder */

  if (!stream.startDecoder(VS1053_CS, VS1053_DCS, VS1053_DREQ) || !stream.isChipConnected()) { 
    Serial.println("Decoder not running");
    while (1) delay(1000);
  }
  Serial.println("decoder running");

  Serial.println("wifi:connecting");
  if (!connectToWifi()) {
    Serial.println("error:WIFI");
  } else {
    stream.setVolume(VOLUME);
    startRadio();
  }
}

void loop() {
  stream.loop();

  if (buttonNext.pressed()) changeRadio(true);
  if (buttonPrevious.pressed()) changeRadio(false);
  saveRadioIdx();
  delay(5);
}

void saveRadioIdx() {
  if (!radioIdxSaved && millis() - lastRadioChange > 10000) {
    Serial.println("saveRadio");
    int savedRadioIdx = preferences.getInt("radioIdx", radioIdx);
    if (savedRadioIdx != radioIdx) {
      Serial.printf("Save radio index %i\n", radioIdx);
      radioIdxSaved = true;
      preferences.putInt("radioIdx", radioIdx);
    }
  }
}

void startRadio() {
  Serial.println(RADIOS[radioIdx]);
  Serial.println(LABELS[radioIdx]);

  stream.connecttohost(RADIOS[radioIdx]);

  Serial.printf("codec: %s\n", stream.currentCodec());
  Serial.printf("bitrate: %lu kbps\n", stream.bitrate());
}

void changeRadio(bool next) {
  if (next) {
    radioIdx = radioIdx < NB_RADIOS - 1 ? radioIdx + 1 : 0;
  } else {
    radioIdx = radioIdx > 0 ? radioIdx - 1 : NB_RADIOS - 1;
  }
  if (stream.isRunning()) {
    stream.stopSong();
    delay(100);
  }
  radioIdxSaved = false;
  lastRadioChange = millis();
  startRadio();
}

void audio_showstation(const char* info) {
    Serial.printf("showstation: %s\n", info);
}

void audio_showstreamtitle(const char* info) {
    Serial.printf("streamtitle: %s\n", info);
}

void audio_eof_stream(const char* info) {
    Serial.printf("eof: %s\n", info);
}
