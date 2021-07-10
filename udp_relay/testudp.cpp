#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <string.h> 

#define PORT 5400
#define PACKET_MAX_SIZE 30

int globalClients = 0;


typedef struct{
    int sock;
    struct sockaddr address;
    int addr_len;
} connection_t;

void *process(void *ptr){
    char buffer[PACKET_MAX_SIZE] = {'\0'};
    int len;
    connection_t *conn;
    long addr = 0;
    int valread;
    int clientnum = globalClients++;

    if(!ptr) pthread_exit(0); 
    
    conn = (connection_t*)ptr;
    
    while((buffer[0] != 'e') || (buffer[1] != 'x') || (buffer[2] != 'i') || (buffer[3] != 't')){
        valread = read(conn->sock,buffer,PACKET_MAX_SIZE);
        if(valread){
            printf("Client %d: %s\n",clientnum,buffer);
        }
    }

    close(conn->sock);
    free(conn);
    pthread_exit(0);
}

int main(){
    int sock = -1;
    struct sockaddr_in address;
    int port = PORT;
    connection_t * connection;
    pthread_t thread;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock <= 0){
        fprintf(stderr, "error: cannot create socket\n");
        return -3;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(sock,(struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0){
        fprintf(stderr, "error: cannot bind socket to port %d\n",port);
        return -4;
    }

    if (listen(sock, 5) < 0){
        fprintf(stderr, "error: cannot listen on port\n");
        return -5;
    }
    
    while (1){
        connection = (connection_t *)malloc(sizeof(connection_t));
        connection->sock = accept(sock, &connection->address, (socklen_t*)&connection->addr_len);
        if (connection->sock <= 0){
            free(connection);
        }
        else{
            pthread_create(&thread, 0, process, (void *)connection);
            pthread_detach(thread);
        }
    }
    
    return 0;
}
