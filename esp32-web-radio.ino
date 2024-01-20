#include "parameters.h"

#include <VS1053.h>
#include <ESP32_VS1053_Stream.h>

#include <WiFi.h>
#include "network.h"

#define SPI_CLK_PIN 18
#define SPI_MISO_PIN 19
#define SPI_MOSI_PIN 23

#define VS1053_CS     5
#define VS1053_DCS    16
#define VS1053_DREQ   4

ESP32_VS1053_Stream stream;


int radioIdx = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("Web Radio");

  // Wait for VS1053 and PAM8403 to power up
  // otherwise the system might not start up correctly
  delay(3000);

  SPI.setHwCs(true);
  SPI.begin(SPI_CLK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN);  /* start SPI before starting decoder */

  Serial.println("wifi:connecting");
  if (!connectToWifi()) {
    Serial.println("error:WIFI");
  } else {
   
    if (!stream.startDecoder(VS1053_CS, VS1053_DCS, VS1053_DREQ) || !stream.isChipConnected())
    {
        Serial.println("Decoder not running");
        while (1) delay(1000);
    };
    Serial.println("decoder running - starting stream");

    stream.setVolume(VOLUME);
    stream.connecttohost(RADIOS[radioIdx]);
    Serial.println(LABELS[radioIdx]);

    Serial.print("codec: ");
    Serial.println(stream.currentCodec());

    Serial.print("bitrate: ");
    Serial.print(stream.bitrate());
    Serial.println("kbps");
  }
}

void loop() {
    stream.loop();
    //Serial.printf("Buffer status: %s\n", stream.bufferStatus());
    delay(5);
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