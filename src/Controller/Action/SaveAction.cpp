#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"

SaveAction::SaveAction(SaveConfirmation confirm_save):
    m_confirm_save{confirm_save} {}

void SaveAction::applyTo(EditorState& state) {
    //TODO check if save succeeded (via exception? and give message if it failed)
    FileHandler::saveFile(state.getFile());

    if (m_confirm_save == SaveConfirmation::YES) {
        state.addTemporaryMessage("Saved all changes!");
    }
}