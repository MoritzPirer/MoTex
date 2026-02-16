#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"

void ParagraphSplittingAction::apply(EditorState& state) {
    state.splitAtCursor();
}
