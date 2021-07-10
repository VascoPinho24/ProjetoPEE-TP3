#include <Arduino.h>
#include <WiFiClient.h>
#include <painlessMesh.h>
#include <meshFunctions.h>
#include <get_mesh_credentials.hpp>

#define   MESH_PREFIX     "hive_mesh"
#define   MESH_PASSWORD   "hivetest123"
#define   MESH_PORT       5555

WiFiClient wifiClient;
painlessMesh  mesh;
Scheduler userScheduler;

void register_peripheral(){
    String registerString = R"({"type":"sensor","parameters":["temperature"],"mac_address":")" + WiFi.macAddress() + R"(","is_mesh":"true"})";

    while(!client.connect("192.168.1.105",4001)){
        Serial.print(".");
        delay(100);
    }

    client.print(registerString);
    peripheralID = client.readString();
    Serial.println(peripheralID);
    client.stop();
}

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

void connect_mesh(const char* mesh_ssid, const char* mesh_pwd){
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
     
    mesh.init(mesh_ssid, mesh_pwd, &userScheduler, 5555 );
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
}



///////////////////

void sendMessage(); // Prototype so PlatformIO doesn't complain
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

size_t getRootId(painlessmesh::protocol::NodeTree nodeTree) {
  if (nodeTree.root) return nodeTree.nodeId;
  for (auto&& s : nodeTree.subs) {
    auto id = getRootId(s);
    if (id != 0) return id;
  }
  return 0;
}

void sendMessage() {
    size_t rootnode;
    String msg = "Hi from node ";
    msg += mesh.getNodeId();
    rootnode = getRootId(mesh.asNodeTree());
    mesh.sendSingle( rootnode,msg );
    Serial.println(rootnode);
    Serial.println("Sent message");
    taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

///////////////////

void setup() {
    Serial.begin(115200);

    if(!SPIFFS.begin()){
        Serial.println("Error initializing SPIFFS");
    }

    // Verify if wifi_credentials exist

    if(SPIFFS.exists(mesh_ssid) && SPIFFS.exists(mesh_pwd)){
        File ssid_file = SPIFFS.open(mesh_ssid, "r");
        File pwd_file = SPIFFS.open(mesh_pwd, "r");

        const char* ssid = ssid_file.readStringUntil('\n').c_str();
        const char* pwd = pwd_file.readStringUntil('\n').c_str();

        Serial.println(ssid);
        Serial.println(pwd);

        ssid_file.close();
        pwd_file.close();
    }else{
        getMeshCredentials();
    }

    //connect_mesh();

    register_peripheral();

    userScheduler.addTask( taskSendMessage );
    taskSendMessage.enable();

}

void loop() {
    mesh.update();
    delay(1000);
}