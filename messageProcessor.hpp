#ifndef FAULTYCHAT_MESSAGEPROCESSOR_HPP
#define FAULTYCHAT_MESSAGEPROCESSOR_HPP

#include <mutex>
#include <queue>
#include <map>
#include <unordered_map>
#include "Algorithms.hpp"
#include "Utility.hpp"

class messageProcessor {
public:
    messageProcessor();
    ~messageProcessor();

    void print();
    void parseMessageQueueItem();
    void placeMessage(char* msg, uint32_t length);

    bool emptyPrint();
    bool emptyMessages();
private:
    //std::unordered_map<char*, uint32_t> message;
    std::queue<char*> toPrint;
    std::queue<std::pair<char*, uint32_t>> messages;
    char* data;
    uint64_t lastUUID;
    uint32_t length;
    uint64_t totalLength;
    uint32_t currentSeg;
    uint32_t totalSeg;
};


#endif //FAULTYCHAT_MESSAGEPROCESSOR_HPP
