#ifndef FAULTYDISK_UTILITY_HPP
#define FAULTYDISK_UTILITY_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream>

#define R_LEN 37
#define D_LEN 49
#define W_LEN 48
#define A_LEN 49

#define PORT_OUT 1982
#define PORT_IN 1983

#define MAXLINE 1024



namespace util{

    void init();

    void end();

    unsigned char* createWrite(const std::string& file, const unsigned int& loc, const unsigned short& len, const char* data);

    bool validWrite (char* ack);

    unsigned char* createRead(const std::string& file, const int& loc);

    bool parseRead(char* data, char* &parsed, short& length);

    void sendUDP(const unsigned char* data);

    unsigned char* getUDP(int& ret);

    char* readBlock(const std::string& path, int& segs, int& remLen);

    void writeBlcok(const std::string& path, unsigned char* memBlock, const int& size);
}

#endif //FAULTYDISK_UTILITY_HPP
