#ifndef FAULTYCHAT_MESSAGEPROCESSOR_HPP
#define FAULTYCHAT_MESSAGEPROCESSOR_HPP

#include <mutex>
#include <queue>
#include <unordered_map>
#include "Algorithms.hpp"

class messageProcessor {
public:
    messageProcessor();
    ~messageProcessor();

    void setHeader(char* msg);
    void placeMessage(char* msg);
private:
    std::queue<char*> messages;
    std::queue<char*> messageParts;
    uint64_t lastUUID;
    uint32_t length;
    uint32_t currentSeg;
    uint32_t totalSeg;
};


#endif //FAULTYCHAT_MESSAGEPROCESSOR_HPP
