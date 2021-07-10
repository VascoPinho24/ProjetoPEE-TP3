#include "../include/crypto.hpp"

static const int G = 5;
static const int P = 23;
//static volatile unsigned long long int priv;
//static volatile unsigned long long int key;

unsigned long long int power(long long int a, long long int b,long long int P){ 
    if (b == 1)
        return a;
    else
        return (((long long int)pow(a, b)) % P);
}

unsigned long long int generatePriv(){
    srand(time(NULL));
    unsigned long long int priv = rand() % 10 + 1;
    return priv;
}

unsigned long long int negotiateKey(int socket){
    uint8_t buffer[4];
    unsigned long long int priv;
    priv = generatePriv();
    std::cout << "Private number: " << priv << std::endl;
    unsigned long long int exchange = power(G,priv,P);
    std::cout << "Exchanged number: "  << exchange << std::endl;

    for(int i = 0; i < 4; i++){
        buffer[i] = ((exchange >> (i * 8)) & 0XFF);
    }

    send(socket,buffer,4,0);
    read(socket,buffer,4);

    exchange = 0;

    for(int i = 0; i < 4; i++){
        exchange += (buffer[i] << (i*8));
    }

    std::cout << "Received number: " << exchange << std::endl;

    unsigned long long int key = power(exchange,priv,P);

    std::cout << "Key negotiated: " << key << std::endl;

    return key;
}

void encrypt(uint8_t* array,int size,unsigned long long int key){
    for(int i=0;i<size;i++){
        //array[i] = ((array[i] + key) % 255);
        array[i] = array[i] ^ key;
    }
}

void decrypt(uint8_t* array,int size,unsigned long long int key){
    for(int i=0;i<size;i++){
        array[i] = array[i] ^ key;
       /* if(array[i] < key){
            array[i] = (256 + array[i]) - key;
        }else{
            array[i] -= key;
        } */
    }
}
