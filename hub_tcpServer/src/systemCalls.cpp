#include "../include/systemCalls.hpp"

uint8_t* getFile(uint32_t* bufferSize,const char* location){
    std::ifstream file(location);
    std::filebuf* pbuf = file.rdbuf();

    if(!file.is_open()){
        std::cout << "Error reading file \n";

        return 0;
    }

    std::size_t size = pbuf->pubseekoff(0,file.end,file.in);
    pbuf->pubseekpos(0,file.in);

    char* filearray = new char[size];

    pbuf->sgetn(filearray,size);

    file.close();

    *bufferSize = size;

    uint8_t* returnFile = reinterpret_cast<uint8_t*>(filearray);

    return returnFile;
}