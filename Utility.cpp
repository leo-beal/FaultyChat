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

unsigned char* util::createWrite(const std::string &file, const unsigned int &loc, const unsigned short &len, const char *data) {
    unsigned char* msg = new unsigned char[W_LEN];

    //Set first byte to W (Write) opcode
    msg[0] = 'W';

    //Set bytes 1 - 32 to the file to write to
    for (int x = 0; x < 32; x++){
        if(file.size() > x){
            msg[x + 1] = file.at(x);
        }
        else{
            msg[x + 1] = 0;
        }
    }

    //Set bytes 33-36 to the location
    memcpy(msg + 33, (unsigned char *)&(loc), sizeof(loc));

    //Set byte 37 to the data length
    msg[37] = len;

    //Set bytes 38-47 to the data (no ending zero padding needed)
    memcpy(msg + 38, data, static_cast<size_t>(len));

    return msg;
}

unsigned char* util::createRead(const std::string &file, const int &loc) {
    unsigned char* msg = new unsigned char[R_LEN];

    //Set first byte to R (Read) opcode
    msg[0] = 'R';

    //Set bytes 1 - 32 to the file to read from
    for (int x = 0; x < 32; x++){
        if(file.size() > x){
            msg[x + 1] = file.at(x);
        }
        else{
            msg[x + 1] = 0;
        }
    }

    //Set bytes 33-36 to the location
    memcpy(msg + 33, (unsigned char *)&(loc), sizeof(loc));

    return msg;
}

bool util::validWrite(char *ack) {
    if(ack[A_LEN - 1] == 0){
        return false;
    }else{
        return true;
    }
}

bool util::parseRead(char *data, char* &parsed, short& length) {
    if(data[D_LEN - 1] == 0){
        return false;
    }else{
        short len = data[37];
        parsed = new char[len];
        for (int x = 0; x < len; x++){
            parsed[x] = data[x + 38];
        }
        //memcpy(parsed, data + 38, len);
        length = len;
        return true;
    }
}

void util::sendUDP(const unsigned char *data) {

    if(data[0] == 'W' || data[0] == 'w') {
        sendto(sockifd, data, W_LEN, 0, (const struct sockaddr *) &servoaddr, sizeof(servoaddr));
    }
    if(data[0] == 'R' || data[0] == 'r') {
        sendto(sockifd, data, R_LEN, 0, (const struct sockaddr *) &servoaddr, sizeof(servoaddr));
    }
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

    if(buffer[0] == 'A' || buffer[0] == 'a'){
        retVal = new unsigned char[A_LEN];
        memcpy(retVal, buffer, A_LEN);
    }
    if(buffer[0] == 'D' || buffer[0] == 'd'){
        retVal = new unsigned char[D_LEN];
        memcpy(retVal, buffer, D_LEN);
    }

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
