#include <ncurses.h>

#include "../../inc/View/DisplayHandler.hpp"
#include "../../inc/Controller/Control/TextRole.hpp"

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


void DisplayHandler::setStyle(TextRole role) {
    attrset(A_NORMAL);
    
    switch (role) {
        case TextRole::TEXT_NORMAL: {
            attron(COLOR_PAIR(1));
            break;
        }
        case TextRole::TEXT_HIGHLIGHT: {
            attron(COLOR_PAIR(2));
            break;
        }
        case TextRole::FILE_CHANGED: {
            attron(COLOR_PAIR(3));
            break;
        }
        case TextRole::FILE_NEW: {
            attron(COLOR_PAIR(4));
            break;
        }
        case TextRole::FILE_SAVED: {
            attron(COLOR_PAIR(5));
            break;
        }
        case TextRole::UI_ELEMENT: {
            attron(COLOR_PAIR(6));
            break;
        }
    }
}

void DisplayHandler::addContent(const std::string& content) {
    addstr(content.c_str());
}