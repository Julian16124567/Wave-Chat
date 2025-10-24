#include <iostream>
#include <string>
#include <ncurses.h>
#include "../../include/tui/tui.hpp"
using std::string;

int main() {
    Tui t;
    t.WindowInit();
    t.ColorMain();
    t.getMaxy();
    t.DrawMain();
};



