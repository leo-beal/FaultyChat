#include <iostream>
#include "win/curses.h"

int main() {
    initscr();

    getch();

    endwin();

    return 0;
}