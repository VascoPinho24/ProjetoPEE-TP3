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
    server.sin_addr.s_addr = inet_addr("20.68.171.3");

    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
	printf("\n Connection failed!\n");
	return 0;
    }

    buffer[0] = 1;

    unsigned int hubID = 453;

    for(int i=0;i < 4; i++){
        buffer[i+1] = (hubID >> i*8) & 0xFF;
    }

    sendto(sock, buffer, 5, 0, (const struct sockaddr *) &server, sizeof(server));

    while(1){
        messageLength = recvfrom(sock, buffer, PACKET_MAX_SIZE, 0, (struct sockaddr*) &server, &structLength);

        if(messageLength < 0){
            break;
        }

        std::cout << "Received packet from " << inet_ntoa(server.sin_addr) << " : " << ntohs(server.sin_port) << std::endl;
    }

    close(sock);
    
    return 0;
}
