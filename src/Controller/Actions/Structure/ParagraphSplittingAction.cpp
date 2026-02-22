#include "../../../../inc/Controller/Actions/Structure/ParagraphSplittingAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

ParagraphSplittingAction::ParagraphSplittingAction(Position first_after_split):
    m_first_after_split{first_after_split}
    {}

void ParagraphSplittingAction::apply(ExecutionContext& context) {
    context.state.splitAt(m_first_after_split);
    context.state.requestBackup();
}

void ParagraphSplittingAction::undo(EditorState& state) {
    state.joinNextParagraphTo(m_first_after_split.row);
    state.moveCursorTo(m_first_after_split);

    state.requestBackup();
}

bool ParagraphSplittingAction::canBeUndone() const {
    return true;
}