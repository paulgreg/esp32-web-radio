#include "parameters.h"
#include "string.h"

#include <Button.h>
#include <Preferences.h>

#include "display.h"

#include <VS1053.h>
#include <ESP32_VS1053_Stream.h>

#include <WiFi.h>
#include "network.h"

#define RADIO_LIMIT 16
#define SONG_LIMIT 42

Preferences preferences;
ESP32_VS1053_Stream stream;

Button buttonNext(BTN_NEXT);
Button buttonPrevious(BTN_PREVIOUS);

unsigned short radioIdx = 0;
bool hasRadioIdxChanged = false;
unsigned long lastAction = millis();
bool radioIdxSaved = true;
char radioLabel[255];
char songLabel[255]; 

void setup() {
  radioLabel[0] = '\0';
  songLabel[0]= '\0';

  Serial.begin(115200);
  Serial.println("Web Radio");

  buttonNext.begin();
  buttonPrevious.begin();

  preferences.begin("webradio", false);
  radioIdx = preferences.getInt("radioIdx", radioIdx);

  setupScreen();
  displayText("Radio");

  delay(3000); // Wait for VS1053 and PAM8403 to power up
  SPI.setHwCs(true);
  SPI.begin(SPI_CLK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);  /* start SPI before starting decoder */

  displayText("Init");

  if (!stream.startDecoder(VS1053_CS, VS1053_DCS, VS1053_DREQ) || !stream.isChipConnected()) { 
    Serial.println("Decoder not running");
    while (1) delay(1000);
  }
  Serial.println("decoder running");

  displayText("Wifi");

  Serial.println("wifi:connecting");
  if (!connectToWifi()) {
    Serial.println("error:WIFI");
    displayError("Wifi error");
  } else {
    stream.setVolume(VOLUME);
    displayText("Lets go");
    startRadio();
  }
}

bool hasTimePassed (unsigned long time) {
  return millis() - lastAction > time;
}

void loop() {    
  if (stream.isRunning()) {
    stream.loop();
    delay(5);
    saveRadioIdx();
  }

  if (buttonNext.pressed()) changeRadioIndex(true);
  else if (buttonPrevious.pressed()) changeRadioIndex(false);
  
  changeRadio();
}

void copyRadioStation(const char* str) {
  unsigned int len = strlen(str);
  if (len == 0) {
    radioLabel[0] = '\0';
  } else {
    unsigned int limit = len > RADIO_LIMIT ? RADIO_LIMIT : len;
    strncpy(radioLabel, str, limit);
    capitalizeWords(radioLabel);
    radioLabel[limit] = '\0';
  }
}

void copyRadioSong(const char* str) {
  unsigned int len = strlen(str);
  if (len == 0) {
    songLabel[0] = '\0';
  } else {
   unsigned int limit = len > SONG_LIMIT ? SONG_LIMIT : len;
   strncpy(songLabel, str, limit);
    capitalizeWords(songLabel);
    songLabel[limit] = '\0';
  }
}

void saveRadioIdx() {
  if (stream.isRunning() && !radioIdxSaved && hasTimePassed(SAVE_RADIO_IDX_DELAY)) {
    Serial.println("should saveRadio");
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

  copyRadioStation(LABELS[radioIdx]);
  copyRadioSong("");
  displayData(radioLabel, songLabel);
  stream.connecttohost(RADIOS[radioIdx]);

  Serial.printf("codec: %s\n", stream.currentCodec());
  Serial.printf("bitrate: %lu kbps\n", stream.bitrate());
}

void changeRadioIndex(bool next) {
  if (stream.isRunning()) stream.stopSong();
  if (next) {
    radioIdx = radioIdx < NB_RADIOS - 1 ? radioIdx + 1 : 0;
  } else {
    radioIdx = radioIdx > 0 ? radioIdx - 1 : NB_RADIOS - 1;
  }
  copyRadioStation(LABELS[radioIdx]);
  copyRadioSong("");
  displayData(radioLabel, songLabel);
  hasRadioIdxChanged = true;
  lastAction = millis();
}

void changeRadio () {
  if (hasRadioIdxChanged && hasTimePassed(CHANGE_RADIO_DELAY)) {
    hasRadioIdxChanged = false;
    radioIdxSaved = false;
    lastAction = millis();
    startRadio();
  }
}

void audio_showstation(const char* station) {
  Serial.printf("showstation: %s\n", station);
  char * aac = strstr(station, ".aac");
  char * mp3 = strstr(station, ".mp3");
  if (aac == NULL && mp3 == NULL) {
    copyRadioStation(station);
    displayData(radioLabel, songLabel);
  }
}

void audio_showstreamtitle(const char* song) {
  Serial.printf("streamtitle: %s\n", song);
  copyRadioSong(song);
  displayData(radioLabel, songLabel);
}

void audio_eof_stream(const char* eof) {
  Serial.printf("eof: %s\n", eof);
}
