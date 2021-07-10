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

#define PORT 1234

#include "../include/tcpServer.hpp"

int main(){
    
    spawnServer(9191);

    while(true){
        sleep(10);
    }
    
}