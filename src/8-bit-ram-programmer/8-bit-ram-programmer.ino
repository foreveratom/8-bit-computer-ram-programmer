/**
 * This sketch is for use with Ben Eater's 8-bit computer RAM module WiFi/HTTP programmer.
 * See https://github.com/foreveratom/8-bit-computer-ram-programmer
 * 
 * To check and/or change the pin mapping, edit the "8BitLoader.h" file.
 * 
 */
#include <Arduino.h>
#include "8BitWiFi.h"

//CHANGE ME
const char* SSID = "YOUR SSID";
const char* KEY = "YOUR WIRELESS KEY";

WebServer8B server(SSID, KEY);

void setup() {
  Serial.begin(9600);
  server.begin();  
}

void loop() {
  server.listen();
  yield();
}
