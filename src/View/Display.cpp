#include <ncurses.h>

#include "../../inc/View/Display.hpp"
#include "../../inc/Model/TextFile.hpp"

int Display::screenHeight() {
    return getmaxy(stdscr);
}
int Display::screenWidth() {
    return getmaxx(stdscr);
}

int Display::visualRowsNeeded(size_t line_length) {
    if (line_length == 0) {
        return 1;
    }

    int lines_needed = (line_length + screenWidth() - 1) / screenWidth();
    return lines_needed + (line_length % screenWidth() == 0);
}

void Display::renderEmptyLine(int& visual_row) {
    mvprintw(visual_row, 0, "~");
    visual_row++;
}

int Display::renderLine(int& visual_row, int logical_line_index) {
    int visual_row_of_cursor = 0;
    const std::string& line = m_controller.getLine(logical_line_index);
    mvprintw(visual_row, 0, "%s", line.c_str());
    if (m_controller.getCursorRow() == logical_line_index) {
        visual_row_of_cursor = visual_row + visualRowsNeeded(m_controller.getCursorColumn()) - 1;
    }
    
    visual_row += TextFile::visualLinesNeeded(line.length(), screenWidth());

    return visual_row_of_cursor;
}

int Display::renderText() {
    Position first_visible_char = m_controller.getFirstVisibleChar(screenWidth(), screenHeight());

    const std::string& first_visible_line = m_controller.getPartialLine(first_visible_char);
    mvprintw(0, 0, "%s", first_visible_line.c_str());
    
    size_t visual_row_of_cursor = 0;
    if (m_controller.getCursorRow() == 0) {
        visual_row_of_cursor = visualRowsNeeded(m_controller.getCursorColumn()) - 1;
    }

    int logical_line_index = first_visible_char.row + 1;
    int visual_row = TextFile::visualLinesNeeded(first_visible_line.length(), screenWidth());

    while (visual_row < screenHeight()) {
        if (logical_line_index >= m_controller.getLineCount()) {
            renderEmptyLine(visual_row);
            continue;
        }
        
        int temp = renderLine(visual_row, logical_line_index);
        if (temp != 0) visual_row_of_cursor = temp;

        logical_line_index++;
    }

    return visual_row_of_cursor;
}
 
void Display::renderCursor(size_t visual_row_of_cursor) {
    int cursor_column = m_controller.getCursorColumn();
    
    // DEBUG HELPER
    //size_t cursor_row = m_controller.getCursorRow();
    //mvprintw(screenHeight() - 2, 0, "cursor at (%zu/%zu) (sw = %zu)", cursor_row, cursor_column, screenWidth());
    
    while (cursor_column >= screenWidth()) {
        cursor_column -= screenWidth();
    }
    
    move(visual_row_of_cursor, cursor_column);
    curs_set(2);
}

void Display::render() {
    clear();

    int visual_row_of_cursor = renderText();
    renderCursor(visual_row_of_cursor);

    refresh();
}

void Display::mainLoop() {
    bool quit = false;

    while (!quit) {
        render();

        int input = getch();
        quit = m_controller.processInput(input, screenWidth());
    }
}