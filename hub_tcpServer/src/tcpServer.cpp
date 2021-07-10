#include "../include/tcpServer.hpp"

void spawnServer(int port){
    std::thread thread(tcpServer,port);
    thread.detach();
}

void tcpServer(int port){
    struct sockaddr_in address;
    connection_t* newConnection;

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock <= 0){
        std::cout << "Error: Cannot create socket\n";
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(sock,(struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0){
        std::cout << "Error: Cannot bind to port " << port << std::endl;
        return;
    }
    if (listen(sock, 5) < 0){
        std::cout << "Error: Cannot listen on port\n";
        return;
    }

    while(1){
        newConnection = (connection_t *)malloc(sizeof(connection_t));
        newConnection->socket = accept(sock, &newConnection->address, (socklen_t*)&newConnection->addr_len);
        if (newConnection->socket <= 0){
            delete newConnection;
        }
        else{
            std::thread threadHandler(connectionHandler,newConnection);
            threadHandler.detach();
        }
    }
}

void connectionHandler(connection_t* conn){
    uint8_t buffer[PACKET_MAX_SIZE] = {'\0'};
    int valread;

    while(true){
        valread = read(conn->socket,buffer,PACKET_MAX_SIZE);
        if(valread > 0){
            switch(buffer[1]){
                case 0:
                    // POST
                    break;
                case 1:
                    // GET
                    getHandler(buffer,conn);

                    break;
                case 2:
                    // File transfer
                    break;
            }
        }else{
            break;
        }
    }

    close(conn->socket);
    delete conn;
    pthread_exit(0);
}

void postHandler(uint8_t* buffer,connection_t* conn){
    uint8_t sequenceNumber = buffer[0];

    switch(buffer[1]){
        case 0:

            break;
    }
}

void getHandler(uint8_t* buffer,connection_t* conn){
    uint8_t sequenceNumber = buffer[0];
    uint8_t* responseBuffer;
    uint32_t responseLength;

    switch(buffer[2]){
        case RETRIEVE_PERIPHERAL:
            responseBuffer = getFile(&responseLength,"peripherals.json");
            sendResponse(conn->socket,responseBuffer,responseLength,sequenceNumber);
            break;
        case RETRIEVE_RULES:
            responseBuffer = getFile(&responseLength,"rules.json");
            sendResponse(conn->socket,responseBuffer,responseLength,sequenceNumber);
            break;
    }
}

void transferHandler(uint8_t* buffer,connection_t* conn){
    uint8_t sequenceNumber = buffer[0];

    switch(buffer[1]){
        case 0:

            break;
    }
}

void sendResponse(int socket,uint8_t* buffer,uint32_t responseLength,uint8_t sequenceNumber){
    double length = responseLength;
    uint32_t numPackets = std::ceil(length/(PACKET_MAX_SIZE-1));
    uint8_t responseBuffer[PACKET_MAX_SIZE];
    responseBuffer[0] = sequenceNumber;
    uint8_t* bufferIterator = buffer;

    send(socket,&numPackets,1,0);

    for(int i=0;i<numPackets;i++){
        bufferIterator = buffer+(i*(PACKET_MAX_SIZE-1));
    
        if(((buffer+responseLength)-bufferIterator) < (PACKET_MAX_SIZE-1) ){

            std::copy(bufferIterator, buffer+responseLength, responseBuffer+1);
            send(socket,responseBuffer,(buffer+responseLength)-bufferIterator + 1,0);

        }else{

            std::copy(bufferIterator, bufferIterator + (PACKET_MAX_SIZE-1), responseBuffer+1);
            send(socket,responseBuffer,PACKET_MAX_SIZE,0); 
            
        }
    }

    free(buffer);
}