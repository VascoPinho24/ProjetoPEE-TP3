#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <meshFunctions.h>

WiFiClient client;
PubSubClient mqtt(client);

String peripheralID;

bool connectWifi(const char* ssid, const char* pwd){
    WiFi.disconnect();
    WiFi.begin(ssid,pwd);

    int i = 0;

    while((WiFi.status() != WL_CONNECTED) && i<21){
        Serial.print(".");
        delay(100);
        i++;
    }

    if(WiFi.status() == WL_CONNECTED){
        return true;
    }else{
        return false;
    }
}

bool getWifiCredentials(){
    StaticJsonDocument<256> doc;
    WiFi.begin("HivePairer");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    if(client.connect("192.168.4.1",8888)){
        Serial.println("Connected!");
        client.print("{\"connection_type\":\"wifi\"}");
        deserializeJson(doc, client);
        client.stop();
    }else{
        return false;
    }

    const char* ssid = doc["ssid"];
    const char* password = doc["password"];

    if(connectWifi(ssid,password)){
        return true;
    }

    return false;
}

void register_peripheral(){
    String registerString = R"({"type":"sensor","parameters":["temperature"],"mac_address":")" + WiFi.macAddress() + R"("})";

    while(!client.connect("192.168.1.105",4001)){
        Serial.print(".");
        delay(100);
    }

    client.print(registerString);
    peripheralID = client.readString();
    Serial.println(peripheralID);
    client.stop();
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void connectMQTT(){
    mqtt.setServer("192.168.1.105", 1883);
    mqtt.setCallback(callback);

    while(!mqtt.connected()){
        mqtt.connect(WiFi.macAddress().c_str());
        Serial.print(".");
        delay(100);
    }

    String subscribetopic = "/sensor/" + peripheralID + "/state/";

    Serial.print("Subscribed to");
    Serial.println(subscribetopic);

    mqtt.subscribe(subscribetopic.c_str());
}

void setup() {
    Serial.begin(115200);

    while(!getWifiCredentials());

    register_peripheral();

    connectMQTT();    
}

void loop() {
    String topicStr = "/sensor/" + peripheralID + "/temperature/";
    String temp = String(random(30,35));
    mqtt.publish(topicStr.c_str(), temp.c_str());
    mqtt.loop();
    delay(1000);
}