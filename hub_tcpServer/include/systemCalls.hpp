#include <iostream>
#include <fstream>
#include <stdint.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

enum {
    RETRIEVE_RULES,
    RETRIEVE_PERIPHERAL
};

uint8_t* getFile(uint32_t*,const char*);