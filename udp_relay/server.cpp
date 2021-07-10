#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT1 8080
#define PORT2 8090

int main(){
  struct sockaddr_in serAddr, serAddr2, cliAddr1, cliAddr2;
  int socket1,socket2,readStatus;
  socklen_t cliAddrLen;
  char buff[1024] = {0};

  if ((socket1 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation error...\n");
    exit(-1);
  }

  if ((socket2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation error...\n");
    exit(-1);
  }

  serAddr.sin_family = AF_INET;
  serAddr.sin_port = htons(PORT1);
  serAddr.sin_addr.s_addr = INADDR_ANY;

  serAddr2.sin_family = AF_INET;
  serAddr2.sin_port = htons(PORT2);
  serAddr2.sin_addr.s_addr = INADDR_ANY;


  if ((bind(socket1, (struct sockaddr*)&serAddr, sizeof(serAddr))) < 0) {
    perror("binding error...\n");
    close(socket1);
    exit(-1);
  }

  sleep(20);

  if ((bind(socket2, (struct sockaddr*)&serAddr2, sizeof(serAddr2))) < 0) {
    perror("binding error...\n");
    close(socket2);
    exit(-1);
  }

  cliAddrLen = sizeof(cliAddr1);

  while(1){
    readStatus = recvfrom(socket1, buff, 1024, 0, (struct sockaddr*)&cliAddr1, &cliAddrLen);
    if (readStatus < 0) {
      perror("reading error...\n");
      close(socket1);
      exit(-1);
    } 

    readStatus = sendto(socket2, buff, 1024, 0, (struct sockaddr*)&cliAddr2, cliAddrLen);
    if (readStatus < 0) {
      perror("sending error...\n");
      close(socket2);
      exit(-1);
    }
  }

  close(socket1);
  close(socket2);
  return 0;
}
