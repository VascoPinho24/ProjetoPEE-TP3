/*
#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <painlessMesh.h>
#include <meshFunctions.h>

#define   MESH_PREFIX     "hive_mesh"
#define   MESH_PASSWORD   "hivetest123"
#define   MESH_PORT       5555

WiFiClient client;
painlessMesh  mesh;
Scheduler userScheduler;

String peripheralID;

void connect_mesh(const char* mesh_ssid, const char* mesh_pwd);

void register_peripheral();
/*
{
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
    }else{
        return false;
    }

    const char* ssid = doc["ssid"];
    const char* password = doc["password"];

    Serial.println(ssid);
    Serial.println(password);

    connect_mesh(MESH_PREFIX,MESH_PASSWORD);

    return true;
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

//Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &register_peripheral);

size_t getRootId(painlessmesh::protocol::NodeTree nodeTree) {
  if (nodeTree.root) return nodeTree.nodeId;
  for (auto&& s : nodeTree.subs) {
    auto id = getRootId(s);
    if (id != 0) return id;
  }
  return 0;
}

void register_peripheral() {
    size_t rootnode;
    String msg = R"({"type":"register_peripheral","data":{"type":"sensor","parameters":["temperature"],"mac_address":")" + WiFi.macAddress() + R"("}})";
    msg += mesh.getNodeId();
    rootnode = getRootId(mesh.asNodeTree());
    mesh.sendSingle( rootnode,msg );
    Serial.println(rootnode);
    Serial.println("Sent register");
    //taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

///////////////////

void setup() {
    Serial.begin(115200);

    while(!getMeshCredentials());

    register_peripheral();

    //userScheduler.addTask( taskSendMessage );
    //taskSendMessage.enable();

}

void loop() {
    mesh.update();
    delay(1000);
}
*/

#include <Arduino.h>
#include <WiFiClient.h>
#include <painlessMesh.h>
#include <meshFunctions.h>

#define   MESH_PREFIX     "hive_mesh"
#define   MESH_PASSWORD   "hivetest123"
#define   MESH_PORT       5555

WiFiClient wifiClient;
painlessMesh  mesh;
Scheduler userScheduler;

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

    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
     
    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
    userScheduler.addTask( taskSendMessage );
    taskSendMessage.enable();

    Serial.println(mesh.getNodeId());
}

void loop() {
    mesh.update();
    delay(1000);
}