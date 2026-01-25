#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"

void ParagraphSplittingAction::applyTo(EditorState& state, ScreenSize size) {
    (void) size;
    state.splitAtCursor();
}