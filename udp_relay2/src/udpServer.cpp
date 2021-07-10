#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>

#define PORT 8080
#define PACKET_MAX_SIZE 65536

std::map<unsigned int, sockaddr_in> devicesConnected;

void udpServer(int port){
    struct sockaddr_in server,client;
    int messageLength;
    socklen_t structLength = sizeof(server);
    uint8_t buffer[PACKET_MAX_SIZE];

    int sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock < 0){
        std::cout << "Could not create socket" << std::endl;
        return;
    }

    server.sin_addr.s_addr = inet_addr("10.0.0.4");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if(bind(sock, (struct sockaddr*) &server, sizeof(server)) < 0){
        std::cout << "Binding error" << std::endl;
        return;
    }

    unsigned int hubID;

    while(1){
        messageLength = recvfrom(sock, buffer, PACKET_MAX_SIZE, 0, (struct sockaddr*) &client, &structLength);

	hubID = 0;

        if((messageLength == 5) && (buffer[0] == 1)){   // 1st byte = 1 means register new device. The next 4 bytes are the hub's ID

            for(int i=0;i < 4; i++){
                hubID += (buffer[i+1] << (8*i));
            }

            if(devicesConnected.count(hubID)){
                devicesConnected.erase(hubID);
            }

            devicesConnected.insert(std::pair<unsigned int,sockaddr_in>(hubID,client));

            std::cout << "New device connected watching hub " << hubID << "'s stream" << std::endl;
        }else{

            for(int i=0;i<4;i++){
                hubID += (buffer[i] << (8*i));
            }

            std::cout << "Redirecting packet from ID " << hubID << std::endl;

            sendto(sock, buffer + 4, messageLength - 4, 0, (const struct sockaddr*) &devicesConnected.find(hubID)->second, sizeof(client));
        }
    }
}


int main(){
    udpServer(PORT);
}
