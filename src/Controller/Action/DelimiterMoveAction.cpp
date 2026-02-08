#include "../../../inc/Controller/Action/DelimiterMoveAction.hpp"

DelimiterMoveAction::DelimiterMoveAction(
    ScreenSize size,
    std::string delimiters,
    ActionDirection move_direction,
    EndBehavior end_behavior
):
    m_size{size},
    m_delimiters{delimiters},
    m_move_direction{move_direction},
    m_end_behavior{end_behavior}
    {}

void DelimiterMoveAction::applyTo(EditorState& state) {
    Direction direction = toDirection(m_move_direction);
    bool has_reached_delimiter = false;

    while (state.canCursorMove(direction)) {
        state.moveCursor(direction, m_size.width);

        std::optional<char> character = state.readCharacterAtCursor();

        // handle overhang cursor position
        if (!character.has_value()){
            bool cross_into_next_paragraph = false;
            if (cross_into_next_paragraph) {
                continue;
            }

            state.moveCursor(getOppositeDirection(direction), m_size.width);
            break;
        }

        // stop at first delimiter after moving at least once
        if (m_delimiters.find(*character) != std::string::npos) {
            has_reached_delimiter = true;
            
            if (m_end_behavior == EndBehavior::STOP_BEFORE_END) {
                state.moveCursor(getOppositeDirection(direction), m_size.width);
                break;
            }
        }
        else if (has_reached_delimiter && m_end_behavior == EndBehavior::STOP_AFTER_END) {
           break; 
        }
    }
}