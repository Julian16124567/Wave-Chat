#ifndef TUI_HPP
#define TUI_HPP

#include <string>
using std::string;

struct Tui {
    
    //attributes
    int yMax;
    int xMax;
    int yMaxone;
    int xMaxone;
    WINDOW *winone;

    //functions
    void WindowInit(Tui& t);
    void ColorMain();
    void getMaxy();
    void makeWin();
    void drawWinone(Tui& t);
};

#endif

