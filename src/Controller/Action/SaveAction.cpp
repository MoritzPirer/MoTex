#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"

void SaveAction::applyTo(EditorState& state) {
    FileHandler::saveFile(state.getFile());

    bool confirm_save = true;
    if (confirm_save) {
        state.addTemporaryMessage("Saved all changes!");
    }
}