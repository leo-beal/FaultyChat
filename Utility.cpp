#include <bitset>
#include "Utility.hpp"

int sockofd;
int sockifd;
struct sockaddr_in servoaddr;
struct sockaddr_in serviaddr;

void util::init(int portIn, int portOut){
    // Creating socket file descriptor for output
    //if ( (sockofd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    //    perror("socket creation failed");
    //    exit(EXIT_FAILURE);
    //}

    // Creating socket file descriptor for input
    if ( (sockifd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    //memset(&servoaddr, 0, sizeof(servoaddr));
    //memset(&serviaddr, 0, sizeof(serviaddr));

    // Filling server information
    servoaddr.sin_family = AF_INET;
    servoaddr.sin_port = htons(portOut);
    servoaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serviaddr.sin_family = AF_INET;
    serviaddr.sin_port = htons(portIn);
    serviaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::string sendSTD = "hello";

    char* send = (char*)sendSTD.c_str();
    int time = 1000;
    setsockopt(sockifd, SOL_SOCKET, SO_RCVTIMEO, &time, 4);

    if (bind(sockifd, (struct sockaddr *)&serviaddr, sizeof(serviaddr)) < 0) {
        perror("bind failed");
        close(sockifd);
        goto breakFunc;
    }


    sendto(sockifd, send, 5, 0, (const struct sockaddr *) &servoaddr, sizeof(servoaddr));

    breakFunc:;
}

void util::end(){
    //close(sockofd);
    close(sockifd);
}

unsigned char* util::createMessage(
        const uint64_t& uuid,
        const uint32_t& length,
        const char* data,
        uint64_t& totalLength) {

    // size of opcode + size of uuid + size of length + length
    totalLength = 1 + 8 + 4 + length;
    unsigned char* msg = new unsigned char[totalLength];

    //Set first byte to M (Message) opcode
    msg[0] = 'M';

    //Set bytes 1-8 to the uuid
    memcpy(msg + 1, (unsigned char *)&(uuid), 8);

    //Set bytes 9-12 to the data length
    memcpy(msg + 9, (unsigned char *)&(length), 4);

    //Set bytes 13-10,000(max) to the data of the message
    memcpy(msg + 13, data, length);

    return msg;
}

char* util::createFile(
        const uint64_t& uuid,
        const uint32_t& length,
        const uint32_t part,
        const uint32_t total,
        const std::string name,
        const char* data,
        uint64_t& totalLength){

    // size of opcode + size of uuid + size of length + part + parts + name + length
    totalLength = 1 + 8 + 4 + 4 + 4 + 32 + length;
    char* msg = new char[totalLength];

    //Set first byte to F (File) opcode
    msg[0] = 'F';

    //Set bytes 1-8 to the uuid
    memcpy(msg + 1, (unsigned char *)&(uuid), 8);

    //Set bytes 9-12 to the data length
    memcpy(msg + 9, (unsigned char *)&(length), 4);

    //Set bytes 13-16 to the part data
    memcpy(msg + 13, (unsigned char *)&(part), 4);

    //Set bytes 17-20 to the total parts data
    memcpy(msg + 17, (unsigned char *)&(total), 4);

    //Set bytes 21-42 to the name
    //memcpy(msg + 21, (unsigned char *)(name.c_str()), 32);

    for(int x = 0; x < 32; x++){
        if(x < name.length()) {
            msg[x + 21] = name.at(x);
        }else{
            msg[x + 21] = 0;
        }
    }

    //Set bytes 53-10,052(max) to the data of the message
    memcpy(msg + 53, data, length);

    return msg;
}

//for (i = 0; i < N; i += 4)
//thirty_two[i/4] = bytes[i] | (uint32_t)bytes[i+1] << 8
//| (uint32_t)bytes[i+2] << 16 | (uint32_t)bytes[i+3] << 24;

unsigned char* util::parseMessage(const char *data,
        uint64_t &uuid,
        uint32_t &length,
        uint64_t &totalLength) {

    std::string uuidTemp;
    for(int x = 7; x >= 0; x--){
        uuidTemp += std::bitset<8>(data[x+1]).to_string();
    }
    uuid = std::bitset<64>(uuidTemp).to_ulong();

    std::string lengthTemp;
    for(int x = 3; x >= 0; x--){
        lengthTemp += std::bitset<8>(data[x + 9]).to_string();
    }

    length = std::bitset<32>(lengthTemp).to_ullong();

    totalLength = 1 + 8 + 4 + length;
    unsigned char* msg = new unsigned char[length];

    for(int x = 0; x < length; x++){
        msg[x] = data[x + 13];
    }

    return msg;
}

char* util::parseFile(const char *data,
                               uint64_t& uuid,
                               uint32_t& length,
                               uint32_t& part,
                               uint32_t& total,
                               std::string& name,
                                  uint64_t &totalLength) {

    std::string uuidTemp;
    for(int x = 7; x >= 0; x--){
        uuidTemp += std::bitset<8>(data[x+1]).to_string();
    }
    uuid = std::bitset<64>(uuidTemp).to_ulong();

    std::string lengthTemp;
    for(int x = 3; x >= 0; x--){
        lengthTemp += std::bitset<8>(data[x + 9]).to_string();
    }

    length = std::bitset<32>(lengthTemp).to_ullong();

    std::string partTemp;
    for(int x = 3; x >= 0; x--){
        partTemp += std::bitset<8>(data[x + 13]).to_string();
    }

    part = std::bitset<32>(partTemp).to_ullong();

    std::string totalTemp;
    for(int x = 3; x >= 0; x--){
        totalTemp += std::bitset<8>(data[x + 17]).to_string();
    }

    total = std::bitset<32>(totalTemp).to_ullong();

    totalLength = 1 + 8 + 4 + 4 + 4 + 32 + length;

    for(int x = 0; x < 32; x++){
        if(data[x + 21] != 0) {
            name += data[x + 21];
        }
    }

    char* msg = new char[length];

    for(int x = 0; x < length; x++){
        msg[x] = data[x + 53];
    }

    return msg;
}

void util::sendUDP(const unsigned char *data, const uint64_t& length) {
    sendto(sockifd, data, length, 0, (const struct sockaddr *) &servoaddr, sizeof(servoaddr));
}

unsigned char* util::getUDP(int& ret) {
    unsigned char buffer[MAXLINE];
    unsigned char* retVal;

    ssize_t n;

    n = recv(sockifd, (unsigned char *)buffer, MAXLINE,
                 0);
    buffer[n] = '\0';

    ret = n;

    //std::cout << n << buffer[48] <<std::endl;

    //std::cout << "Got message: " << buffer[0] << std::endl;

    retVal = new unsigned char[ret];
    memcpy(retVal, buffer, ret);

    return retVal;
}

char* util::readBlock(const std::string& path, int& segs, int& remLen) {
    std::streampos size;
    char* block;

    std::ifstream fileIn (path, std::ios::in | std::ios::binary | std::ios::ate);
    if(fileIn.is_open()){
        size = fileIn.tellg();
        block = new char[size];
        fileIn.seekg (0, std::ios::beg);
        fileIn.read (block, size);
        fileIn.close();
    }
    else{
        std::cout << "Unable to open file" << std::endl;
        return nullptr;
    }

    if(size > 50000000){
        std::cout << "File too large" << std::endl;
        return nullptr;
    }

    long temp = size / 10000;
    if(size % 10000 > 0){
        temp += 1;
        remLen = (int)size%10000;
    }
    segs = (int)temp;

    return block;
}

void util::writeBlcok(const std::string& path, unsigned char *memBlock, const int &size) {
    std::ofstream fileOut (path, std::ios::out | std::ios::binary | std::ios::app);
    fileOut.write ((char*)memBlock, size);
    fileOut.close();
}
