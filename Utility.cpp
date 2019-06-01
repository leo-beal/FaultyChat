#include "Utility.hpp"

int sockofd;
int sockifd;
struct sockaddr_in servoaddr;
struct sockaddr_in serviaddr;

void util::init(){
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
    servoaddr.sin_port = htons(PORT_OUT);
    servoaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    serviaddr.sin_family = AF_INET;
    serviaddr.sin_port = htons(PORT_IN);
    serviaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockifd, (struct sockaddr *)&serviaddr, sizeof(serviaddr)) < 0) {
        perror("bind failed");
        close(sockifd);
        goto breakFunc;
    }

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
    remLen = 0;

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

    long temp = size / 10;
    if(size % 10 > 0){
        temp += 1;
        remLen = (int)size%10;
    }
    segs = (int)temp;

    return block;
}

void util::writeBlcok(const std::string& path, unsigned char *memBlock, const int &size) {
    std::ofstream fileOut (path, std::ios::out | std::ios::binary);
    fileOut.write ((char*)memBlock, size);
}
