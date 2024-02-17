#include "parameters.h"
#include "stringUtils.h"

#include "IRremote.hpp"

#include <Preferences.h>

#include "display.h"

#include <VS1053.h>
#include <ESP32_VS1053_Stream.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "WebRadios.h"
#include "network.h"


Preferences preferences;
ESP32_VS1053_Stream stream;


WebRadios webRadios;

unsigned short radioIdx = 0;
bool hasRadioIdxChanged = false;
unsigned long lastAction = millis();
bool radioIdxSaved = true;
unsigned int volume = VOLUME_MAX;
bool volumeSaved = true;
bool mute = false;
bool eof = false;

char radioLabel[255];
char songLabel[255];

boolean fetchWebRadiosData() {
  boolean success = false;
  while(!success) {
    delay(RETRIES_DELAY);
    success = getWebRadiosJSON(&webRadios);
  }
  return success;
}

void setup() {
  radioLabel[0] = songLabel[0] ='\0';

  Serial.begin(115200);
  Serial.println("Web Radio");

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);  

  preferences.begin("webradio", false);
  radioIdx = preferences.getInt("radioIdx", radioIdx);
  volume = preferences.getInt("volume", volume);

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
  
  displayText("Wifi");
  if (!connectToWifi()) {
    displayError("Wifi error");
  } else {
    displayText("Fetch radios");
    if (fetchWebRadiosData()) {
      radioIdx = radioIdx < webRadios.max ? radioIdx : 0;
      stream.setVolume(volume);
      displayText("Lets go");
      startRadio();
    }
  }
}

void loop() {    
  if (stream.isRunning()) {
    stream.loop();
    delay(5);
    savePreferences();
  }
  handleIRCommands();
  changeRadio();
}

void handleIRCommands() {
 if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    Serial.printf("IRcommand: %u\n", command);
    switch (command) { 
      case IR_NEXT: 
        changeRadioIndex(true);
        break;
      case IR_PREVIOUS:
        changeRadioIndex(false);
        break;
      case IR_VOL_UP: 
        changeVolume(true);
        break;
      case IR_VOL_DOWN:
        changeVolume(false);
        break;
      case IR_VOL_MUTE:
        toggleMute();
        break;
      default:	
        Serial.println("IRCommand: unknown");
    }
    delay(100);
		IrReceiver.resume();
  }
}

bool hasTimePassed (unsigned long time) {
  return millis() - lastAction > time;
}

void savePreferences() {
  if (hasTimePassed(SAVE_DELAY)) {
    if (stream.isRunning() && !radioIdxSaved) {
      Serial.println("should save radio index");
      radioIdxSaved = true;
      if (!preferences.isKey("radioIdx") || preferences.getInt("radioIdx", radioIdx) != radioIdx) {
        Serial.printf("!!! Save radio index %i\n", radioIdx);
        preferences.putInt("radioIdx", radioIdx);
      }
    }
     if (!volumeSaved) {
      Serial.println("should save volume");
      volumeSaved = true;
      if (!preferences.isKey("volume") || preferences.getInt("volume", volume) != volume) {
        Serial.printf("!!! Save volume %i\n", volume);
        preferences.putInt("volume", volume);
      }
    }
  }
}

void startRadio() {
  Serial.printf("StartRadio %s - %s\n", webRadios.url[radioIdx], webRadios.name[radioIdx]);
  copyString(webRadios.name[radioIdx], radioLabel);
  copyString("", songLabel);
  eof = false;
  refreshDisplay();
  stream.connecttohost(webRadios.url[radioIdx]);
  Serial.printf("codec: %s - bitrate: %lu kbps\n", stream.currentCodec(), stream.bitrate());
}

void restartRadio() {
  Serial.printf("RestartRadio %s - %s\n", webRadios.url[radioIdx], webRadios.name[radioIdx]);
  eof = false;
  stream.connecttohost(webRadios.url[radioIdx]);
}

void changeRadioIndex(bool next) {
  if (stream.isRunning()) stream.stopSong();
  if (next) radioIdx = radioIdx < webRadios.max - 1 ? radioIdx + 1 : 0;
  else radioIdx = radioIdx > 0 ? radioIdx - 1 : webRadios.max - 1;
  copyString(webRadios.name[radioIdx], radioLabel);
  copyString("", songLabel);
  refreshDisplay();
  hasRadioIdxChanged = true;
  lastAction = millis();
}

void toggleMute() {
  mute = stream.getVolume() == volume;
  unsigned int v = mute ? 0 : volume;
  stream.setVolume(v);
  refreshDisplay();
}

void changeVolume(bool increase) {
  if (!mute) {
    volume = stream.getVolume();
    if (increase && volume <= (VOLUME_MAX - VOLUME_STEP)) volume += VOLUME_STEP;
    else if (!increase && volume >= VOLUME_STEP) volume -= VOLUME_STEP;
    volumeSaved = false;
    lastAction = millis();
    stream.setVolume(volume);
    refreshDisplay();
  }
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
  char* aac = strstr(station, ".aac");
  char* mp3 = strstr(station, ".mp3");
  if (aac == NULL && mp3 == NULL) {
    copyString(station, radioLabel);
    refreshDisplay();
  }
}

void audio_showstreamtitle(const char* song) {
  Serial.printf("streamtitle: %s\n", song);
  copyString(song, songLabel);
  refreshDisplay();
}

void audio_eof_stream(const char* error) {
  Serial.printf("End of stream: %s\n", error);
  eof = true;
  refreshDisplay();
  delay(1000);
  restartRadio();
}

void refreshDisplay() {
  displayData(radioLabel, songLabel, volume, mute, eof);
}
