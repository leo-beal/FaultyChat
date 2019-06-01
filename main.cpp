#include <iostream>
#include "win/curses.h"

int main() {
    initscr();

    getch();

    endwin();

    std::cout << "Ur gay" << std::endl;

    return 0;
}