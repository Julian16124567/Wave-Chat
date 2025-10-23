#include <iostream>
#include <string>
#include <ncurses.h>
#include "../../include/tui/tui.hpp"
using std::string;

//initialise windwow
void Windowinit() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

//set color/colorpairs
void ColorMain() {
    if(!has_colors()) {
        std::cerr << "[*] Terminal does not support colors!" << std::endl;
        endwin();  
    } else {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_CYAN);
        clear();
        
    }
}

//get terminal height/width
void getMaxy(Tui& t) {
    getmaxyx(stdscr, t.yMax, t.yMax);
}

//draw main window
void drawMain(Tui& t) {
    WINDOW *winone = newwin(t.yMax / 2, t.yMax / 2, 1, 1);
    box(winone, 0 , 0);

}

