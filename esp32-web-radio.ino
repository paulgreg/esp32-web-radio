#include "parameters.h"

#include <SPI.h>
#include <Adafruit_VS1053.h>
#include "player.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include "network.h"

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("Web Radio");

  if (!player.begin()) {
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  } 

  player.begin();
  player.setVolume(30,30); // Set volume for left, right channels. lower numbers == louder volume!

  Serial.println("wifi:connecting");
  if (!connectToWifi()) {
    Serial.println("error:WIFI");
  } else {
     Serial.printf("HTTP: %s:%i%s\n", RADIO_HOST, RADIO_PORT, RADIO_PATH);
    if (!client.connect(RADIO_HOST, RADIO_PORT)) {
      Serial.println("error:HTTP:connect");
      return;
    }
    Serial.println("HTTP:connect");
    client.print(String("GET ") + RADIO_PATH + " HTTP/1.1\r\n" +
               "Host: " + RADIO_HOST + "\r\n" +
               "Connection: close\r\n\r\n");
    Serial.println("HTTP:connected");
  }
}


#define SIZE 32
uint8_t mp3buff[SIZE];

void loop() {
  if (player.readyForData()) {
    if (client.available() > 0) {
      uint8_t bytesread = client.read(mp3buff, SIZE);
      player.playData(mp3buff, bytesread);
    }
  } 
}
