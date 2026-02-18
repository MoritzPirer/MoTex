#include "../../../../inc/Controller/Actions/Structure/ParagraphJoiningAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

ParagraphJoiningAction::ParagraphJoiningAction(Position joining_position):
    m_joining_position{joining_position},
    m_first_of_joinee{std::nullopt}
    {}

void ParagraphJoiningAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;
    int row = m_joining_position.row;

    if (static_cast<size_t>(row) < state.getNumberOfParagrahps() - 1) {
        m_first_of_joinee = {row, static_cast<int>(state.getParagraph(row).length())};
        state.joinNextParagraphTo(row);
    }

}

void ParagraphJoiningAction::undo(EditorState& state) {
    if (m_first_of_joinee.has_value()) {
        state.splitAt(*m_first_of_joinee);
    }
}

bool ParagraphJoiningAction::canBeUndone() const {
    return true;
}