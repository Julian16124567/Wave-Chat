#include <iostream>
#include <string>
#include <ncurses.h>
#include "../../include/tui/tui.hpp"
using std::string;

//initialise windwow
void WindowInit(Tui& t) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    WINDOW *winone = newwin(t.yMax / 2, t.yMax / 2, 1, 1);
    box(t.winone, 0 , 0);
}

//set color/colorpairs
void ColorMain() {
    if(!has_colors()) {
        std::cerr << "[*] Terminal does not support colors!" << std::endl;
        endwin();  
    } else {
        std::cout << "[*] Colors initialized." << std::endl;
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_CYAN);
        clear();
        
    }
}


//get terminal height/width
void getMaxy(Tui& t, int inp, WINDOW* winone) {
    if (inp == 1) {
        getmaxyx(stdscr, t.yMax, t.xMax);
    } else if (inp == 2) {
        getmaxyx(winone, t.yMaxone, t.xMaxone);
    }
}

//draw main window/winone
void drawWinone(Tui& t) {
    getMaxy(t, 2, t.winone);

}


//make windowone
void makeWin(Tui& t,string winname, int a, int b, int c, int d) {
    WINDOW *winname = newwin(a, b, c, d);
}