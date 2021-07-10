#include <meshFunctions.h>

painlessMesh  mesh;
WiFiClient wifiClient;
SoftwareSerial hubSerial(RX_PIN, TX_PIN);
//HardwareSerial hubSerial(2);

String meshName;
String meshPassword;
uint32_t meshPort = 5555;

bool initMesh(){
    hubSerial.begin(9600);

    if(!getCredentials()){
        return false;
    }

    mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages
    mesh.init(meshName, meshPassword, meshPort, WIFI_AP_STA, 6 );
    mesh.onReceive(&receivedCallback);
    mesh.setRoot(true);
    mesh.setContainsRoot(true);

    return true;
}

bool getCredentials(){
    hubSerial.println("{\"type\":\"get_credentials\"}");

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, hubSerial);

    String mesh_name = doc["ssid"];
    String mesh_password = doc["password"];

    if(mesh_name != "null" || mesh_password != "null"){
        meshName = mesh_name;
        meshPassword = mesh_password;

        return true;
    }else{
        return false;
    }
}

void receivedCallback(const uint32_t &from, const String &msg ) {
    hubSerial.println(msg);
    mesh.sendSingle(from, hubSerial.readString());
}

void meshLoop(){
    mesh.update();
}