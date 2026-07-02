#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200); //ajustar baudios
  Serial.println(WiFi.macAddress());
}

void loop() {}


