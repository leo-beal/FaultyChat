#include <iostream>
#include <thread>
#include <windows.h>
#include "Algorithms.hpp"
#include "Utility.hpp"
#include "messageProcessor.hpp"
#include "win/curses.h"

bool running;
int increment;
std::string workingPath;
int copies;

void sender(){
    while (running){
        std::string readin;
        std::getline(std::cin, readin);
        //std::cin >> readin;
        if(readin == "exit"){
            running = false;
        }
        else if(readin.substr(0, 2) == "/F"){
            std::string path = workingPath + "/" + readin.substr(3, readin.length() - 3);
            std::string name = readin.substr(3, readin.length() - 3);
            int segs;
            int rem;
            char* file = util::readBlock(path, segs, rem);
            if(file != nullptr){
                std::vector<char*> parts = algo::vectorize(file, segs, rem);
                for(int x = 0; x < parts.size() - 1; x++){
                    uint64_t total = 0;
                    auto data = util::createFile(
                            increment,
                            10000,
                            x,
                            parts.size() - 1,
                            name,
                            parts[x],
                            total);
                    auto send = algo::encodeHamming((char *) data, total);
                    for(int y = 0; y < copies; y++) {
                        util::sendUDP((unsigned char *) send, total);
                    }
                }
                if(rem > 0){
                    uint64_t total = 0;
                    auto data = util::createFile(
                            increment,
                            rem,
                            parts.size() - 1,
                            parts.size() - 1,
                            name,
                            parts[parts.size() - 1],
                            total);
                    auto send = algo::encodeHamming((char *) data, total);
                    for(int y = 0; y < copies; y++) {
                        util::sendUDP((unsigned char *) send, total);
                    }
                }else{
                    uint64_t total = 0;
                    auto data = util::createFile(
                            increment,
                            10000,
                            parts.size() - 1,
                            parts.size() - 1,
                            name,
                            parts[parts.size() - 1],
                            total);
                    auto send = algo::encodeHamming((char *) data, total);
                    for(int y = 0; y < copies; y++) {
                        util::sendUDP((unsigned char *) send, total);
                    }
                }
            }
            increment++;
        }
        else if(readin.substr(0, 2) == "/C"){
            copies = std::stoi(readin.substr(3));
        }
        else {
            uint64_t total = 0;
            auto data = util::createMessage(increment, readin.length(), readin.c_str(), total);
            auto send = algo::encodeHamming((char *) data, total);
            for(int x = 0; x < copies; x++) {
                util::sendUDP((unsigned char *) send, total);
            }
            increment++;
            delete data;
            delete send;
        }
    }
}

void recvAndParse(messageProcessor& proc, bool rec){
    while(running){
        if(rec){
            int ret;
            auto msg = util::getUDP(ret);
            if(ret > 1){
                proc.placeMessage((char*)msg, ret);
            }
        }else{
            if(!proc.emptyPrint()){
                proc.print();
            }
            if(!proc.emptyMessages()){
                proc.parseMessageQueueItem();
            }
            //std::cout << "Nothing to process" << std::endl;
            Sleep(500);
        }
    }
}

void reciver(messageProcessor proc){
    while (running){
        int ret;
        auto msg = util::getUDP(ret);
        if(ret > 5){
            proc.placeMessage((char*)msg, ret);
        }
    }
}

void parser(messageProcessor proc){
    while(running){
        if(!proc.emptyMessages()){
            proc.parseMessageQueueItem();
        }
        if(!proc.emptyPrint()){
            proc.print();
        }
        //std::cout << "Nothing to process" << std::endl;
        Sleep(500);
    }
}

int main(int argc, char* argv[]) {

    int portIn = 0;
    int portOut = 0;
    copies = 20;

    increment = 10;

    for(int x = 0; x < argc; x++){
        if(std::string(argv[x]) == "-pI"){
            try {
                portIn = std::stoi(std::string(argv[x+1]));
                x++;
            }catch(...){
                std::cout << "Improper usage of -pI." << std::endl;
                return 0;
            }
        }
        if(std::string(argv[x]) == "-pO"){
            try{
                portOut = std::stoi(std::string(argv[x+1]));
                x++;
            }catch(...){
                std::cout << "Improper usage of -pO." << std::endl;
                return 0;
            }
        }
        if(std::string(argv[x]) == "-c"){
            try{
                copies = std::stoi(std::string(argv[x+1]));
                x++;
            }catch(...){
                std::cout << "Improper usage of -c." << std::endl;
                return 0;
            }
        }
        if(std::string(argv[x]) == "-w"){
            workingPath = std::string(argv[x+1]);
            x++;
        }
    }

    util::init(portIn, portOut);

    messageProcessor proc(workingPath);

    running = true;

    //initscr();

    std::thread send(sender);
    std::thread recive(recvAndParse, std::ref(proc), true);
    std::thread parse(recvAndParse, std::ref(proc), false);

    //getch();

    send.join();
    recive.join();
    parse.join();

   //endwin();

    return 0;
}