#include "../../../../inc/Controller/Actions/System/QuitAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

QuitAction::QuitAction(QuitMode quit_mode):
    m_quit_mode{quit_mode}
    {}

void QuitAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;

    if (m_quit_mode != QuitMode::FORCE_QUIT && state.getSaveState() != SaveState::SAVED) {
        state.addTemporaryMessage("You have unsaved changes! Save your changes [!s] or quit without saving [!x]!");
        return;
    }
    state.setIsQuit(true);
}