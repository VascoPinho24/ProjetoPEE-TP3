#include <stdint.h>
#include <Arduino.h>

void receivedCallback( uint32_t from, String &msg );
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);