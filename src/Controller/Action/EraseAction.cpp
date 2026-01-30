#include "../../../inc/Controller/Action/EraseAction.hpp"

EraseAction::EraseAction(int offset): m_offset{offset} {}

void EraseAction::applyTo(EditorState& state) {
    Position cursor_position = state.getCursor().getPosition();
    Position erase_position = cursor_position; 
    
    if (m_offset < 0) {
        // move left if possible
        if (erase_position.column > 0) {
            // state.debug(std::to_string(erase_position.column));
            erase_position.column--;
        }
        // otherwise move to end of prev if possible
        else if (erase_position.row > 0) {
            erase_position.row--;
            erase_position.column = state.getParagraph(erase_position.row).length() - 1;
        } 
        // if already at (0/0) erase offset is impossible
        else {
            return;
        }
        
    }
    else if (m_offset > 0) {
        return; //TODO
    }

    if (erase_position.row < cursor_position.row) {
        state.joinLineToPrevious(cursor_position.row);
    }
    else if (static_cast<size_t>(erase_position.column) == state.getParagraph(erase_position.row).length()
        && static_cast<size_t>(cursor_position.row + 1) < state.getNumberOfParagrahps()) {

        state.joinLineToPrevious(cursor_position.row + 1);
        state.moveCursorRight();
    }
    else {
        state.deleteRange(erase_position, erase_position);
        if (m_offset < 0) {
            state.moveCursorLeft();
        }
    }
}