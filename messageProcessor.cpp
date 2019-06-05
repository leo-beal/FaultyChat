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
    std::cout << msg << std::endl;
}

void messageProcessor::parseMessageQueueItem() {
    auto msg = messages.front();
    messages.pop();
    std::cout << "recived bytes are" << std::endl;
    for(int x = 0; x < 36; x++){
        std::cout << std::bitset<8>(msg.first[x]).to_string() << std::endl;
    }
    data = algo::decodeHamming(msg.first, msg.second);
    std::cout << "original bytes are" << std::endl;
    for(int x = 0; x < 18; x++){
        std::cout << std::bitset<8>(data[x]).to_string() << std::endl;
    }
    std::cout << msg.second << " " << data[0] << std::endl;
    if(data[0] == 'M') {
        uint64_t uuidNext;
        auto final = util::parseMessage(data, uuidNext, length, totalLength);

        if(uuidNext != lastUUID){
            std::cout << "Pushing info with length " << length << " and total Length " << totalLength << std::endl;
            toPrint.push((char*)final);
            lastUUID = uuidNext;
        }
    }
}

void messageProcessor::placeMessage(char* msg, uint64_t mlength){
    std::pair<char*, uint64_t> message(msg, mlength);
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