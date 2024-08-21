#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

const char* wifiid = "ESP32 FROM THE PROJECT 2405";
const char* wifipass = "ADOROAPROJETE";

WebServer server(80);

void setup() {
  WiFi.softAP(wifiid, wifipass);
  server.begin();
}

void loop() {
  delay(10000);
}

