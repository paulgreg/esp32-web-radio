#include "parameters.h"

#include <SPI.h>
#include <VS1053.h>
#include "player.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include "network.h"

WiFiClient client;

#define VOLUME 70 // MAX 100

#define BUFFER_SIZE 32  // The buffer size 64 seems to be optimal. At 32 and 128 the sound might be brassy.
uint8_t mp3buff[BUFFER_SIZE];

uint8_t remove_chunk_control_data(uint8_t *data, size_t length);

void setup() {
  Serial.begin(115200);
  Serial.println("Web Radio");

  // Wait for VS1053 and PAM8403 to power up
  // otherwise the system might not start up correctly
  delay(3000);

  SPI.begin();

  player.begin();
  player.switchToMp3Mode();
  player.setVolume(VOLUME);

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
               "Icy-MetaData: 1\r\n" +
               "Connection: close\r\n\r\n");
    Serial.println("HTTP:connected");
  }
}

void loop() {
  if (!client.connected()) {
      Serial.println("Reconnecting...");
      if (client.connect(RADIO_HOST, RADIO_PORT)) {
          client.print(String("GET ") + RADIO_PATH + " HTTP/1.1\r\n" +
                        "Host: " + RADIO_HOST + "\r\n" +
                        "Icy-MetaData: 1\r\n" +
                        "Connection: close\r\n\r\n");
      }
  }

  if (client.available() > 0) {
    uint8_t bytesread = client.read(mp3buff, BUFFER_SIZE);
    bytesread = remove_chunk_control_data(mp3buff, bytesread);
    player.playChunk(mp3buff, bytesread);
  }
}

// Introduce here a new helper buffer with size of 8 bytes, to remove the chunked control bytes.
// Variables must be aligned to avoid the Exception (9) being thrown from the ESP8266
// See here: https://arduino-esp8266.readthedocs.io/en/latest/exception_causes.html
// See here: https://arduino.stackexchange.com/questions/67442/nodemcu-1-0-exception-9-fatal-exception-9loadstorealignmentcause
uint8_t __attribute__((aligned(4))) helperBuffer[8];
uint8_t __attribute__((aligned(4))) helperBufferCount = 0;

/***
 * Removes the chunk control data from the helper buffer.
 *
 * Only the following chunk control bytes are removed:
 * \r\n<byte>\r\n
 * \r\n<byte><byte>\r\n
 * \r\n<byte><byte><byte>\r\n
 * \r\n<byte><byte><byte><byte>\r\n
 *
 */
void remove_chunk_control_data_from_helper_buffer() {
  if (helperBuffer[0] != '\r') {
    // die fast
    return;
  }
  if (helperBuffer[1] != '\n') {
    // die fast
    return;
  }


  if (helperBuffer[3] == '\r' && helperBuffer[4] == '\n') {
    // 5 bytes length chunk control section discovered
    // \r\n<byte>\r\n
    helperBufferCount = 3;
    Serial.println("Removed control data: 5 bytes");

    return;
  }

  if (helperBuffer[4] == '\r' && helperBuffer[5] == '\n') {
    // 6 bytes length chunk control section discovered
    // \r\n<byte><byte>\r\n
    helperBufferCount = 2;
    Serial.println("Removed control data: 6 bytes");

    return;
  }

  if (helperBuffer[5] == '\r' && helperBuffer[6] == '\n') {
    // 7 bytes length chunk control section discovered
    // \r\n<byte><byte><byte>\r\n
    helperBufferCount = 1;
    Serial.println("Removed control data: 7 bytes");

    return;
  }

  if (helperBuffer[6] == '\r' && helperBuffer[7] == '\n') {
    // 8 bytes length chunk control section discovered
    // \r\n<byte><byte><byte><byte>\r\n
    helperBufferCount = 0;
    Serial.println("Removed control data: 8 bytes");

    return;
  }
}

/***
 * Puts a byte to the input of helper buffer and returns a byte from the output of helper buffer.
 * In the meantime it tries to remove the chunk control data (if any available) from the buffer.
 *
 * @param incoming byte of the audio stream
 * @return outgoing byte of the audio stream (if available) or -1 when no bytes available (this happens
 * when the buffer is being populated with the data or after removal of chunk control data)
 */
int16_t put_through_helper_buffer(uint8_t newValue) {
  //
  int16_t result = -1;

  if (helperBufferCount == 8) {
    result = helperBuffer[0];
  }

  helperBuffer[0] = helperBuffer[1];
  helperBuffer[1] = helperBuffer[2];
  helperBuffer[2] = helperBuffer[3];
  helperBuffer[3] = helperBuffer[4];
  helperBuffer[4] = helperBuffer[5];
  helperBuffer[5] = helperBuffer[6];
  helperBuffer[6] = helperBuffer[7];
  helperBuffer[7] = newValue;

  helperBufferCount++;
  if (helperBufferCount > 8) {
    helperBufferCount = 8;
  }

  remove_chunk_control_data_from_helper_buffer();

  return result;
}

/***
 * Removes the chunk control data from the input audio stream. Data are written back to the input
 * buffer and the number of bytes available (after processing) is returned.
 *
 * @see https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Transfer-Encoding
 * @see https://en.wikipedia.org/wiki/Chunked_transfer_encoding
 *
 * @param data a pointer to the input buffer of audio stream
 * @param length a number of input bytes to be processed
 * @return the number of available bytes in the input buffer after removing the chunk control data
 */
uint8_t remove_chunk_control_data(uint8_t *data, size_t length) {

  uint8_t writeindex = 0;
  uint8_t index = 0;
  for (index = 0; index < length; index++) {
    uint8_t input = data[index];
    int16_t output = put_through_helper_buffer(input);
    if (output >= 0) {
      data[writeindex] = (uint8_t) output;
      writeindex++;
    }
  }

  return writeindex;
}
