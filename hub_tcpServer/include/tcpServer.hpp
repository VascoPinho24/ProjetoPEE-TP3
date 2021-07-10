#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <map>
#include <cmath>

#include "systemCalls.hpp"

#define PACKET_MAX_SIZE 40

typedef struct{
    int socket;
    struct sockaddr address;
    int addr_len;
} connection_t;

void spawnServer(int);
void tcpServer(int);
void connectionHandler(connection_t*);
void postHandler(uint8_t*,connection_t*);
void getHandler(uint8_t*,connection_t*);
void transferHandler(uint8_t*,connection_t*);
void sendResponse(int,uint8_t*,uint32_t,uint8_t);