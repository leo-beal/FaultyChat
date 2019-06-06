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

#define MAXLINE 21000



namespace util{

    void init(int portIn, int portOut);

    void end();

    unsigned char* createMessage(
            const uint64_t& uuid,
            const uint32_t& length,
            const char* data,
            uint64_t& totalLength);

    unsigned char* parseMessage(
            const char* data,
            uint64_t& uuid,
            uint32_t& length,
            uint64_t& totalLength
            );

    char* createFile(
            const uint64_t& uuid,
            const uint32_t& length,
            const uint32_t part,
            const uint32_t total,
            const std::string name,
            const char* data,
            uint64_t& totalLength);

    char* parseFile(const char *data,
                                   uint64_t& uuid,
                                   uint32_t& length,
                                   uint32_t part,
                                   uint32_t total,
                                   std::string name,
                                   uint64_t &totalLength);

    void sendUDP(const unsigned char* data, const uint64_t& length);

    unsigned char* getUDP(int& ret);

    char* readBlock(const std::string& path, int& segs, int& remLen);

    void writeBlcok(const std::string& path, unsigned char* memBlock, const int& size);
}

#endif //FAULTYDISK_UTILITY_HPP
