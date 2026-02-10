#include "../../../inc/Controller/Action/InsertAction.hpp"

InsertAction::InsertAction(char character_to_add):
    m_character_to_add{character_to_add} {}

void InsertAction::applyTo(EditorState& state) {
    state.insertCharacterAt(m_character_to_add, state.getCursor().getPosition());
}