#include <get_mesh_credentials.hpp>

bool getMeshCredentials(){
    StaticJsonDocument<256> doc;
    WiFi.begin("HivePairer");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    if(client.connect("192.168.4.1",8888)){
        Serial.println("Connected!");
        client.print("{\"connection_type\":\"mesh\"}");
        deserializeJson(doc, client);
        client.stop();
    }

    const char* ssid = doc["ssid"];
    const char* password = doc["password"];

    Serial.println(ssid);
    Serial.println(password);

    connect_mesh(ssid,password);

    if(1){
        SPIFFS.format();

        File ssid_file = SPIFFS.open(mesh_ssid,"w");
        File pwd_file = SPIFFS.open(mesh_pwd,"w");

        ssid_file.println(ssid);
        pwd_file.println(password);

        ssid_file.close();
        pwd_file.close();
        return true;
    }

    return false;
}