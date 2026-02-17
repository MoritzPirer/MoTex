#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

ParagraphSplittingAction::ParagraphSplittingAction(Position first_after_split):
    m_first_after_split{first_after_split}
    {}

void ParagraphSplittingAction::apply(ExecutionContext& context) {
    context.state.splitAt(m_first_after_split);
}

void ParagraphSplittingAction::undo(EditorState& state) {
    state.joinNextParagraphTo(m_first_after_split.row);
    state.moveCursorTo(m_first_after_split);
}

bool ParagraphSplittingAction::canBeUndone() const {
    return true;
}