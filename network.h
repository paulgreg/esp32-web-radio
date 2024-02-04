boolean connectToWifi() {
  if (WiFi.status() == WL_CONNECTED) return true;
  
  Serial.print("\nconnecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(false);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.reconnect();
  unsigned int retries = 100;
  while (WiFi.status() != WL_CONNECTED && (retries-- > 0)) {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWifi connection failed");
    return false;
  }
  Serial.println("");
  Serial.println("wifi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.println("");
  return true;
}

String httpGet(const char* url, const char* login, const char* password) {
  String s = "";
  if ((WiFi.status() == WL_CONNECTED)) {
    Serial.print("Connecting to "); Serial.println(url);
    HTTPClient http;
    http.begin(url);
    if (strlen(login) > 0 && strlen(password) > 0) http.setAuthorization(login, password);
    int httpCode = http.GET();
     Serial.print("HTTP code : "); Serial.println(httpCode);
    if (httpCode > 0) {
      s = http.getString();
      Serial.print("Reponse length : "); Serial.println(s.length());
      Serial.println(s);
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }  
  return s;
}

boolean getWebRadiosJSON(WebRadios* webRadios) {
  boolean success = false;
  String data = httpGet(WEB_RADIOS_URL, WEB_RADIO_USER, WEB_RADIO_PASSWD);
  JSONVar json = JSON.parse(data);
  if (JSON.typeof(json) == "undefined") {
    Serial.println("Parsing data input failed!");
  } else {
    fillWebRadiosFromJson(json, webRadios);
    success = true;
  }
  return success;
}
