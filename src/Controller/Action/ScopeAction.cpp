#include "../../../inc/Controller/Action/ScopeAction.hpp"

ScopeAction::ScopeAction(
    ScreenSize size,
    EndBehavior end_behavior
):
    m_size{size},
    m_end_behavior{end_behavior}
    {}

Position ScopeAction::startOfScope(const EditorState& state, Scope scope) {
    switch (scope) {
    case Scope::FILE: {
        return {0,0};
    }

    case Scope::PARAGRAPH: {
        switch (m_end_behavior) {
        case EndBehavior::STOP_AFTER_END:
        case EndBehavior::STOP_ON_END: {
            int row = state.getCursor().getRow();
            if (row > 0) {
                row--;
                return {row, static_cast<int>(state.getParagraph(row).length() - 1)};
            }

            return {row, 0};
        }

        case EndBehavior::STOP_BEFORE_END: {
            return {state.getCursor().getRow(), 0};
        }
        }
    }

    case Scope::LINE: {
        switch (m_end_behavior) {
        case EndBehavior::STOP_AFTER_END:
        case EndBehavior::STOP_ON_END: {
            int row = state.getCursor().getRow();
            int new_column = state.getCursor().getColumn() / m_size.width * m_size.width;
            if (new_column == 0) { 
                if (row > 0) {
                    return {row - 1, static_cast<int>(state.getParagraph(row - 1).length() - 1)};
                }
                else {
                    return {row, new_column};
                }
            }
            else {
                return {row, new_column - 1};
            }
        }

        case EndBehavior::STOP_BEFORE_END: {
            int new_column = state.getCursor().getColumn() / m_size.width * m_size.width;
            return {state.getCursor().getRow(), new_column};
        }
        }

    }

    default: throw std::invalid_argument("nuh uh");
    }
}

Position ScopeAction::endOfScope(const EditorState& state, Scope scope) {
    switch (scope) {
    case Scope::FILE: {
        int row = state.getNumberOfParagrahps() - 1;
        return {row, static_cast<int>(state.getParagraph(row).length() - 1)};
    }

    case Scope::PARAGRAPH: {
        switch (m_end_behavior) {
        case EndBehavior::STOP_AFTER_END:
        case EndBehavior::STOP_ON_END: {
            int row = state.getCursor().getRow();
            if (static_cast<size_t>(row) < state.getNumberOfParagrahps() - 1) {
                row++;
                return {row, 0};
            }
            
            return {row, static_cast<int>(state.getParagraph(row).length() - 1)};
        }

        case EndBehavior::STOP_BEFORE_END: {
            int row = state.getCursor().getRow();
            int column = std::max(static_cast<int>(state.getParagraph(row).length() - 1), 0);
            return {row, column};
        }
        }
    }

    case Scope::LINE: {
        switch (m_end_behavior) {
        case EndBehavior::STOP_AFTER_END:
        case EndBehavior::STOP_ON_END: {
            int row = state.getCursor().getRow();
            int last_of_current = state.getCursor().getColumn() / m_size.width * m_size.width + m_size.width - 1;
            last_of_current = std::min(
                static_cast<size_t>(last_of_current),
                state.getParagraph(state.getCursor().getRow()).length()
            );
            
            if (static_cast<size_t>(last_of_current) == state.getParagraph(row).length()) {
                if (static_cast<size_t>(row) == state.getNumberOfParagrahps() - 1) {
                    return {row, last_of_current};
                }
                else {
                    return {row + 1, 0};
                }
            }
            else {
                return {row, last_of_current + 1};
            }
        }

        case EndBehavior::STOP_BEFORE_END: {
            int new_column = state.getCursor().getColumn() / m_size.width * m_size.width + m_size.width - 1;
            new_column = std::min(
                static_cast<size_t>(new_column),
                state.getParagraph(state.getCursor().getRow()).length()
            );

            return {state.getCursor().getRow(), new_column};
        }
        }

    }

    default: {
        throw std::invalid_argument("nah");
    }
    }
}