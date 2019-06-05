#include <iostream>
#include <thread>
#include <windows.h>
#include "Algorithms.hpp"
#include "Utility.hpp"
#include "messageProcessor.hpp"
#include "win/curses.h"

bool running;
int increment;

void sender(){
    while (running){
        std::string readin;
        std::cin >> readin;
        if(readin == "exit"){
            running = false;
        }else {
            uint64_t total = 0;
            auto data = util::createMessage(increment, readin.length(), readin.c_str(), total);
            std::cout << "original bytes are" << std::endl;
            for(int x = 0; x < total; x++){
                std::cout << std::bitset<8>(data[x]).to_string() << std::endl;
            }
            auto send = algo::encodeHamming((char *) data, total);
            std::cout << "Sending packet of length " << total << std::endl;
            std::cout << "bytes are" << std::endl;
            for(int x = 0; x < total; x++){
                std::cout << std::bitset<8>(send[x]).to_string() << std::endl;
            }
            util::sendUDP((unsigned char *) send, total);
            increment++;
        }
    }
}

void recvAndParse(messageProcessor& proc, bool rec){
    while(running){
        if(rec){
            int ret;
            auto msg = util::getUDP(ret);
            if(ret > 1){
                std::cout << "Got message" << std::endl;
                proc.placeMessage((char*)msg, ret);
            }
        }else{
            if(!proc.emptyPrint()){
                std::cout << "atempting print" << std::endl;
                proc.print();
            }
            if(!proc.emptyMessages()){
                std::cout << "atempting parse" << std::endl;
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
        if(ret > 1){
            std::cout << "Got message" << std::endl;
            proc.placeMessage((char*)msg, ret);
        }
    }
}

void parser(messageProcessor proc){
    while(running){
        if(!proc.emptyMessages()){
            std::cout << "atempting parse" << std::endl;
            proc.parseMessageQueueItem();
        }
        if(!proc.emptyPrint()){
            std::cout << "atempting print" << std::endl;
            proc.print();
        }
        //std::cout << "Nothing to process" << std::endl;
        Sleep(500);
    }
}

int main(int argc, char* argv[]) {

    int portIn = 0;
    int portOut = 0;

    increment = 10;

    for(int x = 0; x < argc; x++){
        if(std::string(argv[x]) == "-pI"){
            try {
                portIn = std::stoi(std::string(argv[x+1]));
                x++;
            }catch(...){
                std::cout << "Improper usage of -n. Defaulting to 3 copies." << std::endl;
            }
        }
        if(std::string(argv[x]) == "-pO"){
            try{
                portOut = std::stoi(std::string(argv[x+1]));
                x++;
            }catch(...){
                std::cout << "Improper usage of -t. Defaulting to 30 seconds" << std::endl;
            }
        }
    }

    util::init(portIn, portOut);

    messageProcessor proc(1);

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