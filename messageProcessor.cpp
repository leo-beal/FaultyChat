#include "messageProcessor.hpp"

messageProcessor::messageProcessor() {
    lastUUID = 0;
    length = 0;
    totalLength = 0;
    currentSeg = 0;
    totalSeg = 0;
}

messageProcessor::messageProcessor(int test) {
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
    std::cout << "Them: " << msg << std::endl;
    delete p;
}

void messageProcessor::parseMessageQueueItem() {
    auto msg = messages.front();
    messages.pop();
    data = algo::decodeHamming(msg.first, msg.second);
    if(data[0] == 'M') {
        uint64_t uuidNext;
        auto final = util::parseMessage(data, uuidNext, length, totalLength);

        if(uuidNext != lastUUID){
            toPrint.push((char*)final);
            lastUUID = uuidNext;
        }else{
            delete final;
        }
    }
}

void messageProcessor::placeMessage(char* msg, uint64_t mlength){
    std::pair<char*, uint64_t> message(msg, mlength);
    messages.push(message);
}

bool messageProcessor::emptyMessages() {
    return messages.empty();
}

bool messageProcessor::emptyPrint() {
    return toPrint.empty();
}