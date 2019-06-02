#ifndef FAULTYCHAT_MESSAGEPROCESSOR_HPP
#define FAULTYCHAT_MESSAGEPROCESSOR_HPP

#include <mutex>
#include <queue>
#include <unordered_map>

class messageProcessor {
public:
    messageProcessor();
    ~messageProcessor();

    void uuid(char* msg);

private:
    std::queue<char*> messages;
    uint64_t lastUUID;
    uint32_t currentSeg;
    uint32_t totalSeg;
};


#endif //FAULTYCHAT_MESSAGEPROCESSOR_HPP
