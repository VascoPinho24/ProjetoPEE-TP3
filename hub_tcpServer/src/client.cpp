#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <string>

enum {
    RETRIEVE_RULES,
    RETRIEVE_PERIPHERAL
};

int main(){
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    uint8_t buffer[1024] = {'\0'};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9191);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
        return -1;
    }

    buffer[0] = 32; //SEQ number
    buffer[1] = 1;  //GET
    buffer[2] = RETRIEVE_RULES;  //PERIPHERAL LIST

    send(sock,buffer,3,0);

    valread = read(sock,buffer,1024);
    printf("%d \n",buffer[0]);

    std::string str;

    do{
        valread = read(sock,buffer,40);
        for(int i=1;i<valread;i++){
           str += buffer[i];
        }
    }while(valread>0);

    std::cout << str << std::endl;

    return 0;
}