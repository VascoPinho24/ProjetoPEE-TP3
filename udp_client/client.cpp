#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <stdlib.h> 

int main(int argc, char** argv) 
{ 
    int sock,i=0,bytes_read; 
    struct sockaddr_in server_addr; 
    struct sockaddr_in client_addr2; 
    struct hostent *host; 
    char send_data[1024],recv_data[1024];
    char nickname[20];
  int   slen = sizeof(server_addr);
    if(argc != 2)  
    {  
        printf("Usage : %s <Server-IP>\n",argv[0]);  
        exit(0);  
    }  

    printf("Enter a nickname:");
    scanf("%s",&nickname);

    printf("WELCOME %s ..Please (Enter) to start chat or Quit(q): ",&nickname);
    host= (struct hostent *) gethostbyname((char *)argv[1]); 


    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    { 
        perror("socket"); 
        exit(1); 
    } 

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(5000); 
   // server_addr.sin_addr = inet_addr(argv[1]); //IP ADDRESS 

    server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(server_addr.sin_zero),8); 

   
    
    while (1) 
    { 
        if(i>1)
        {
            printf("%s:",&nickname); 
        }
        fgets(send_data,1024,stdin); 

        if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0) 
            break; 
        else
        {
            sendto(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
            

      // recvfrom(sock, send_data, strlen(send_data), 0,(struct sockaddr *)&server_addr, (socklen_t*)&slen); 
       // recv_data[bytes_read] = '\0';
        }
    i++;
    } 
} 
