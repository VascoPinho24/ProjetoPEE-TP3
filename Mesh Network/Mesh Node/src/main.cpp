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