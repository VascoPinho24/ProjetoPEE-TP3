#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define PACKET_MAX_SIZE 30

int main(){
    int sock;
    struct sockaddr_in server;
    socklen_t structLength = sizeof(server);
    int messageLength;
    uint8_t buffer[PACKET_MAX_SIZE];

    char* hello = "Hello!";

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock < 0){
        std::cout << "Could not create socket" << std::endl;
        return 0;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    unsigned int hubID = 453;

    for(int i=0;i < 4; i++){
        buffer[i+1] = (hubID >> i*8) & 0xFF;
    }

    buffer[4] = 'h';
    buffer[5] = 'e';
    buffer[6] = 'l';
    buffer[7] = 'l';
    buffer[8] = 'o';
    buffer[9] = '\n';

    while(1){
        sendto(sock, buffer, 10, 0, (const struct sockaddr *) &server, sizeof(server));
        sleep(2);
    }

    close(sock);
    
    return 0;
}