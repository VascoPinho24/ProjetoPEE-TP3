#include <Arduino.h>
#include <meshFunctions.h>

void setup() {

    while(!initMesh()){
        delay(100);
    }
    
}

void loop() {
    meshLoop();
    return;
}