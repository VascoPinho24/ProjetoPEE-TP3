#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial hubSerial(13, 15); //D7 for RX, D8 for TX
WiFiServer server(8888);
WiFiClient client;
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
    hubSerial.begin(9600);
    WiFi.config(local_IP,gateway,subnet);
    WiFi.softAP("HivePairer","",1,1);
    server.begin();
}

void loop() {
    StaticJsonDocument<256> doc;

    client = server.available();

    if(client.available() > 0){
        deserializeJson(doc, client);
        serializeJson(doc, hubSerial);
        hubSerial.print('\n');
        deserializeJson(doc, hubSerial);
        serializeJson(doc, client);
    }

    delay(50);
}