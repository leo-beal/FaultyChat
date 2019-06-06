#include "messageProcessor.hpp"

messageProcessor::messageProcessor() {
    lastUUID = 0;
    length = 0;
    totalLength = 0;
    currentSeg = -1;
    totalSeg = 0;
}

messageProcessor::messageProcessor(std::string path) {
    lastUUID = 0;
    length = 0;
    totalLength = 0;
    currentSeg = -1;
    totalSeg = 0;
    workingPath = path;
}

messageProcessor::~messageProcessor() {

}

void messageProcessor::write(){
    while(!toWrite.empty()) {
        char *p = toWrite.front().first;
        int length = toWrite.front().second;
        util::writeBlcok(workingPath + "/" + name, (unsigned char *) p, length);
        toWrite.pop();
    }
    std::cout << "You got a file" << std::endl;
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
    if(data[0] == 'F'){
        uint64_t uuidNext;
        uint32_t nextSeg;
        uint32_t nextTotal;
        std::string nextName;
        auto final = util::parseFile(data, uuidNext, length, nextSeg, nextTotal, nextName, totalLength);

        if(uuidNext != lastUUID && nextSeg != currentSeg){
            toWrite.push(std::pair((char*)final, length));
            //lastUUID = uuidNext;
            currentSeg = nextSeg;
            totalSeg = nextTotal;
            name = nextName;
        }
        if(currentSeg == totalSeg){
            lastUUID = uuidNext;
            currentSeg = -1;
            write();
        }
        else{
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