#ifndef FAULTYCHAT_MESSAGEPROCESSOR_HPP
#define FAULTYCHAT_MESSAGEPROCESSOR_HPP

#include <mutex>
#include <queue>
#include <unordered_map>
#include "Algorithms.hpp"
#include "Utility.hpp"

class messageProcessor {
public:
    messageProcessor();
    ~messageProcessor();

    void print();
    void parseMessageQueueItem();
    void placeMessage(char* msg);

    bool emptyPrint();
    bool emptyMessages();
private:
    std::queue<char*> toPrint;
    std::queue<char*> messages;
    char* data;
    uint64_t lastUUID;
    uint32_t length;
    uint64_t totalLength;
    uint32_t currentSeg;
    uint32_t totalSeg;
};


#endif //FAULTYCHAT_MESSAGEPROCESSOR_HPP
