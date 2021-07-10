#include <stdint.h>
#include <Arduino.h>
#include <painlessMesh.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define RX_PIN 13 // D7
#define TX_PIN 15 // D8

bool initMesh();
bool getCredentials();
void receivedCallback(const uint32_t &from, const String &msg );
void meshLoop();