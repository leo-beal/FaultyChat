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

void messageProcessor::parseMessageQueueItem() {
    char* msg = messages.front();
    messages.pop();
    if(msg[0] == 'M') {
        uint64_t uuidNext;
        data = (char *) util::parseMessage(msg, uuidNext, length, totalLength);
        if(uuidNext != lastUUID){
            toPrint.push(data);
            lastUUID = uuidNext;
        }
    }
}

void messageProcessor::placeMessage(char* msg){
    messages.push(msg);
}

bool messageProcessor::emptyMessages() {
    return messages.empty();
}

bool messageProcessor::emptyPrint() {
    return toPrint.empty();
}