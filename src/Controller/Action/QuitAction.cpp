#include "../../../inc/Controller/Action/QuitAction.hpp"

void QuitAction::applyTo(EditorState& state) {
    state.setIsQuit(true);
}