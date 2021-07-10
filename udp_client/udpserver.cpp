#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <stdlib.h> 

int main() 
{ 
    int sock; 
    int addr_len, bytes_read; 
    char recv_data[1024]; 
    char recv_data2[1024]; 
    struct sockaddr_in server_addr , client_addr,client_addr2,client_addr1; 

    struct sockaddr_in copy;
    char *hello = "PrintSERVER\n";

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    { 
        perror("Socket"); 
        exit(1); 
    } 

struct User
{
    char user_id[20];
    struct sockaddr_in copy;
}U[8];

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(5000); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(server_addr.sin_zero),8); 


    if (bind(sock,(struct sockaddr *)&server_addr, 
    sizeof(struct sockaddr)) == -1) 
    { 
        perror("Bind"); 
        exit(1); 
    } 

    addr_len = sizeof(struct sockaddr); 


    printf("\nUDPServer Waiting for client on port 5000"); 
    fflush(stdout); 

    recvfrom(sock,recv_data2,1024,0,(struct sockaddr *)&client_addr,(socklen_t*) &addr_len);
    
    client_addr2 = client_addr;
    sendto(sock,recv_data2,1024,0,(struct sockaddr *)&client_addr2,addr_len); 
    
    //printf("Client_addr2:   %s",client_addr2);
    //printf("%s",recv_data);
    while (1) 
    { 

    bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr1,(socklen_t*) &addr_len); 
       // (struct sockaddr *)&client_addr1 == (struct sockaddr *)&client_addr;


        recv_data[bytes_read] = '\0'; 
        if(strlen(recv_data)>1)
        {
            printf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port)); 
         
            printf("%s", recv_data); 
            fflush(stdout); 
            sendto(sock,(char*)recv_data,1024,0,(struct sockaddr *)&client_addr2,addr_len); 
   
        }
       
       
        

    } 
 return 0; 
} 
