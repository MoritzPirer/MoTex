#include "inc/Controller/Control/EditorController.hpp"

int main(int argument_count, char* argument_values[]) { 
    switch (argument_count) {
    case 1: {
        EditorController controller;
        controller.mainLoop();
        break;
    }

    case 2: {
        EditorController controller(argument_values[1]);
        controller.mainLoop();
        break;
    }

    }

    return 0;
}