#include "../../../inc/Controller/Action/InsertAction.hpp"

InsertAction::InsertAction(std::vector<std::string> content, Position start):
    m_content{content},
    m_start{start}
    {}

void InsertAction::apply(EditorState& state) {
    state.insertLines(m_content, m_start);

    Position end_cursor_pos = m_start;
    end_cursor_pos.row += m_content.size() - 1;
    // end_cursor_pos.column = m_content.at(m_content.size() - 1).length();

    end_cursor_pos.column = m_content.back().length() + (m_content.size() == 1? m_start.column : 0);

    state.moveCursorTo(end_cursor_pos);

}

void InsertAction::undo(EditorState& state) {

}