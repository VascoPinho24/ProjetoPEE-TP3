#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <fstream>

unsigned long long int power(long long int,long long int,long long int);
unsigned long long int generatePriv();
unsigned long long int negotiateKey(int);
void encrypt(uint8_t*,int,unsigned long long int);
void decrypt(uint8_t*,int,unsigned long long int);

/*
    THIS LIBRARY IS INTENDED ONLY FOR PROOF OF CONCEPT ABOUT
    KEY-EXCHANGE AND ENCRYPTING PACKETS FOR TCP AND UDP 
    COMMUNICATION
*/
