#include "../../../inc/Controller/Action/QuitAction.hpp"

QuitAction::QuitAction(QuitMode quit_mode):
    m_quit_mode{quit_mode}
    {}

void QuitAction::applyTo(EditorState& state) {
    if (m_quit_mode != QuitMode::FORCE_QUIT && state.getSaveState() != SaveState::SAVED) {
        state.addTemporaryMessage("You have unsaved changes! Save or use [shortcut] to quit without saving!");
        return;
    }
    state.setIsQuit(true);
}