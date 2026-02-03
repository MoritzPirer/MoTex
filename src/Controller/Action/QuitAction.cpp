#include "../../../inc/Controller/Action/QuitAction.hpp"

QuitAction::QuitAction(bool force_quit):
    m_force_quit{force_quit}
    {}

void QuitAction::applyTo(EditorState& state) {
    if (m_force_quit == false && state.getSaveState() != SaveState::SAVED) {
        state.addTemporaryMessage("You have unsaved changes! Save or use [shortcut] to quit without saving!");
        return;
    }
    state.setIsQuit(true);
}