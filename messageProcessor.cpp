#include "messageProcessor.hpp"

messageProcessor::messageProcessor() {
    lastUUID = 0;
    length = 0;
    totalLength = 0;
    currentSeg = 0;
    totalSeg = 0;
}

messageProcessor::~messageProcessor() {

}

void messageProcessor::print() {
    char* p = toPrint.front();
    toPrint.pop();
    std::string msg(p, length);
    std::cout << msg << std::endl;
}

void messageProcessor::parseMessageQueueItem() {
    auto msg = messages.front();
    messages.pop();
    if(msg.first[0] == 'M') {
        uint64_t uuidNext;
        data = algo::decodeHamming(msg.first, msg.second);
        auto final = util::parseMessage(data, uuidNext, length, totalLength);

        if(uuidNext != lastUUID){
            toPrint.push((char*)final);
            lastUUID = uuidNext;
        }
    }
}

void messageProcessor::placeMessage(char* msg, uint32_t length){
    std::pair<char*, uint32_t> message(msg, length);
    messages.push(message);
    std::cout << "Pushed message" << std::endl;
    if(messages.empty()){
        std::cout << "Still empty" << std::endl;
    }
}

bool messageProcessor::emptyMessages() {
    return messages.empty();
}

bool messageProcessor::emptyPrint() {
    return toPrint.empty();
}