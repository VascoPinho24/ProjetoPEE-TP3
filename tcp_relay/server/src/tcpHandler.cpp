#include "../include/tcpHandler.hpp"
#include "../include/DBconnection.hpp"
#include "../include/crypto.hpp"

std::map<unsigned int, connection_t*> hubConnections;
std::map<unsigned int, connection_t*> appConnections;
std::map<connection_t*, unsigned long long int> keys;

void spawnServerThread(int port){
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
    unsigned char buffer[PACKET_MAX_SIZE] = {'\0'};
    int valread;
    unsigned char appOrHub; // 0 for app 1 for hub

    if(!conn) pthread_exit(0);

    keys.insert(std::pair<connection_t*,unsigned long long int>(conn,negotiateKey(conn->socket)));

    valread = read(conn->socket,buffer,PACKET_MAX_SIZE);

    std::cout << "Key: " << keys.find(conn)->second << std::endl;

    std::cout << "Buffer: ";

    for(int i=0;i<13;i++){
	    printf("%d ",buffer[i]);
    }

    std::cout << std::endl;

    decrypt(buffer,valread,keys.find(conn)->second);
    if(!valread){
        close(conn->socket);
        delete conn;
        pthread_exit(0);
    }

    std::cout << "Buffer: ";

    for(int i=0;i<13;i++){
	    printf("%d ",buffer[i]);
    }

    std::cout << std::endl;

    int receivedKey = 0;
    int hubId = 0;

    for(int i=0;i<8;i++){
	    receivedKey <<= 8;
        receivedKey |= buffer[5+i];
    }

    for(int i=0;i<4;i++){
        hubId <<= 8;
        hubId |= buffer[i+1];
    }

    std::cout << "Hub id: " << hubId << "\n key: " << receivedKey << std::endl;

    if(receivedKey != hubKey(hubId)){
	std::cout << "Failed to authenticate!\n";
        close(conn->socket);
        delete conn;
        pthread_exit(0);
    }

    std::cout << "Authentication sucessful\n";

    if(buffer[0] == 0){ //App connection
        appOrHub = 0;
        unsigned char msg = 1;
	    std::cout << "New App connection!\n";
        if(!hubConnections.count(hubId)){
            close(conn->socket);
            delete conn;
            pthread_exit(0);
        }
        if(appConnections.count(hubId)){
            appConnections.erase(hubId);
        }
        appConnections.insert(std::pair<int,connection_t*>(hubId,conn));
    }else if(buffer[0] == 1){ //Hub connection
        appOrHub = 1;
	    std::cout << "New Hub connection!\n";
        if(hubConnections.count(hubId)){
            hubConnections.erase(hubId);
        }
        hubConnections.insert(std::pair<int,connection_t*>(hubId,conn));
    }else{
        close(conn->socket);
        delete conn;
        pthread_exit(0);
    }

    while(1){
        valread = read(conn->socket,buffer,PACKET_MAX_SIZE);
        decrypt(buffer,valread,keys.find(conn)->second);
        if(valread > 0){
            if(!appOrHub){
		        //std::cout << "Sent to hub\n";
                encrypt(buffer,valread,keys.find(hubConnections.find(hubId)->second)->second);
                send(hubConnections.find(hubId)->second->socket,buffer,valread,0);
            }else{
		        //std::cout << "Sent to app\n";
                encrypt(buffer,valread,keys.find(appConnections.find(hubId)->second)->second);
                send(appConnections.find(hubId)->second->socket,buffer,valread,0);
            }
        }else{
            break;
        }
    }

    close(conn->socket);
    delete conn;
    pthread_exit(0);
}
