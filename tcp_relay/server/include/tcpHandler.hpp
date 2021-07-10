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

#define PACKET_MAX_SIZE 30

typedef struct{
    int socket;
    struct sockaddr address;
    int addr_len;
    long long int key;
} connection_t;

void spawnServerThread(int);
void connectionHandler(connection_t*);
void tcpServer(int);