#include <ncurses.h>

#include "../../inc/View/InputHandler.hpp"
#include "../../inc/Shared/SpecialInputs.hpp"

int InputHandler::getInput() {
    return getch();
}

int InputHandler::translateInput(int original_input) {
    switch (original_input) {
        case '\n':
        case '\r':
        case KEY_ENTER:
            return INPUT_ENTER;
        
        case 27:
            return INPUT_ESCAPE;
        
        case KEY_BACKSPACE:
        case 127:
        case '\b':
            return INPUT_BACKSPACE;

        default:
            return original_input;
    }
}