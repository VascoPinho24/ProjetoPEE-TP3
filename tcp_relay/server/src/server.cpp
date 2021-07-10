#include "../include/tcpHandler.hpp"
#include "../include/DBconnection.hpp"
#include "../include/crypto.hpp"
#include <unistd.h>

int main(){

    connectDB();
    spawnServerThread(8080);

    while(1){
	    sleep(10);
    }

    return 1;
}
