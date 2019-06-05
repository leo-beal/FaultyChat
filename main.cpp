#include <iostream>
#include <thread>
#include "Algorithms.hpp"
#include "Utility.hpp"
#include "win/curses.h"

bool running;

void sender(){
    while (running){

    }
}

void reciver(){
    while (running){

    }
}

void parser(){
    while(running){

    }
}

int main() {

    util::init();

    running = true;

    initscr();

    std::thread send(sender);
    std::thread recive(reciver);
    std::thread parse(parser);

    getch();

    send.join();
    recive.join();
    parse.join();

    endwin();

    return 0;
}