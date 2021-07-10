#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include "../include/crypto.hpp"
#define PORT 8080
#define PACKET_MAX_SIZE 30

unsigned long long int key;

void listener(int socket){
    uint8_t buffer[PACKET_MAX_SIZE];
    int valread;

    do{
        valread = read(socket,buffer,PACKET_MAX_SIZE);
        decrypt(buffer,valread,key);
        for(int i=0;i<valread;i++){
            std::cout << buffer[i];
        }
        std::cout << std::endl;
    }while(valread > 0);

    close(socket);
    pthread_exit(0);
}

void writer(int socket){
    uint8_t buffer[PACKET_MAX_SIZE];
    char bufferChar[PACKET_MAX_SIZE];
    std::string entry;
    
    while(1){
        std::cin >> entry;
        strncpy(bufferChar,entry.c_str(),PACKET_MAX_SIZE-1);
        for(int i=0;i<entry.size();i++){
            buffer[i] = (unsigned char)bufferChar[i];
        }
        encrypt(buffer,entry.size(),key);
        send(socket,buffer,entry.size(),0);
    }

    close(socket);
    pthread_exit(0);

}

int main(){
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    uint8_t buffer[1024] = {'\0'};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "20.68.171.3", &serv_addr.sin_addr)<=0){
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
        return -1;
    }

    key = negotiateKey(sock);

    uint8_t input;

    do{
        std::cout << "Hub(1) or App(0) ?\n";
        std::cin >> input;
        input -= '0';    
    }while((input != 1) && (input != 0));

    int keyHub = 651589294;
    int hubId = 2;
    buffer[0] = 0;

    for (int i = 0; i < 4; i++){
        buffer[i+1] = hubId >> ((3-i)*8) & 0xFF;
    }

    for (int i = 0; i < 8; i++){
        buffer[i+5] = keyHub >> ((7-i)*8) & 0xFF;
    }

    encrypt(buffer,13,key);
    send(sock,buffer,13,0);

    std::thread threadWriter(writer,sock);
    threadWriter.detach();

    std::thread threadListener(listener,sock);
    threadListener.join();

    return 0;
}

