#include "../../../../inc/Controller/Actions/Editing/InsertAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

InsertAction::InsertAction(std::vector<std::string> content, Position start):
    m_content{content},
    m_start{start}
    {}

void InsertAction::apply(ExecutionContext& context) {
    context.state.insertLines(m_content, m_start);

    Position first_after_insert = {
        static_cast<int>(m_start.row + m_content.size() - 1),
        static_cast<int>(m_content.back().length() + (m_content.size() == 1? m_start.column : 0))
    };

    context.state.moveCursorTo(first_after_insert);
}

bool InsertAction::canBeUndone() const {
    return true;
}

void InsertAction::undo(EditorState& state) {
    Position last_inserted = {
        static_cast<int>(m_start.row + m_content.size() - 1),
        static_cast<int>(m_content.back().length() + (m_content.size() == 1? m_start.column : 0) - 1)
    };
    // state.addTemporaryMessage("content: " + m_content.at(0) + " --- " + std::to_string(m_content.size()));
    state.deleteRange(m_start, last_inserted);
    state.moveCursorTo(m_start);
}


bool InsertAction::canAbsorb(const std::shared_ptr<Action>& action) const {
    auto insert_action = std::dynamic_pointer_cast<InsertAction>(action);

    if (insert_action == nullptr) {
        return false; // not an insert action
    }

    if (insert_action->m_content.empty() || insert_action->m_content.front().empty()) {
        return false; // don't merge empty action
    }

    // TODO: check if they are connected

    // don't merge if not in the same word
    return insert_action->m_content.front().front() != ' ';
}

void InsertAction::absorb(const std::shared_ptr<Action>& action) {
    auto insert_action = std::static_pointer_cast<InsertAction>(action);

    // merge first line of other to last of self
    if (!m_content.empty() && ! insert_action->m_content.empty()) {
        this->m_content.back().append(insert_action->m_content.at(0));
    }

    // append other lines
    for (size_t i = 1; i < insert_action->m_content.size(); i++) {
        this->m_content.push_back(insert_action->m_content.at(i));
    }
}