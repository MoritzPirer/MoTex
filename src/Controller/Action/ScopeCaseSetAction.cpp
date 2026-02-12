#include "../../../inc/Controller/Action/ScopeCaseSetAction.hpp"

ScopeCaseSetAction::ScopeCaseSetAction(
    ScreenSize size,
    Scope scope,
    Case target_case
):
    ScopeAction(size, EndBehavior::STOP_BEFORE_END),
    m_scope{scope},
    m_target_case{target_case}
    {}

void ScopeCaseSetAction::applyTo(EditorState& state) {
    Position start_of_scope = startOfScope(state, m_scope);
    Position end_of_scope = endOfScope(state, m_scope);

    for (int row = start_of_scope.row; row <= end_of_scope.row; row++) {
        if (state.getParagraph(row).length() == 0) {
            continue;
        }

        int start_column = (row == start_of_scope.row? start_of_scope.column : 0);
        int end_column =
            (row == end_of_scope.row? end_of_scope.column : state.getParagraph(row).length() - 1);

        for (int column = start_column; column <= end_column; column++) {
            if (static_cast<size_t>(column) >= state.getParagraph(row).length()) {
                continue;
            }

            char character = *state.readCharacterAt({row, column});

            switch (m_target_case) {
            case Case::UPPER_CASE: {
                character = std::toupper(character);
                break;
            }

            case Case::LOWER_CASE: {
                character = std::tolower(character);
                break;
            }
            }

            state.setCharacterAt(character, {row, column});
        }
    }
}
