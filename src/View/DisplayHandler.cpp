#include <ncurses.h>

#include "../../inc/View/DisplayHandler.hpp"

ScreenSize DisplayHandler::screenSize() const {
    return {getmaxy(stdscr), getmaxx(stdscr)};
}

void DisplayHandler::renderCursor(Position screen_position) {
    move(screen_position.row, screen_position.column);
    curs_set(1);
}

void DisplayHandler::renderLine(int start_visual_row, const std::string& line) {
    mvprintw(start_visual_row, 0, "%s", line.c_str());
}

void DisplayHandler::renderModeLabel(const std::string& label) {
    move(screenSize().height - 1, 0);
    clrtoeol(); //clear line
    mvprintw(screenSize().height - 1, 0, "--%s--", label.c_str());
}