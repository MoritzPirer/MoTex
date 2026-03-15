///
/// @file: EscapeState.hpp
/// @description: description
///
/// @date: 2026-03-14
/// @author: Moritz Pirer
///

#ifndef ESCAPE_STATE_HPP
#define ESCAPE_STATE_HPP

struct EscapeState {
    bool is_style_change_disabled;
    bool has_read_partial_modifier;
    bool has_read_escape_char;
};

#endif //ESCAPE_STATE_HPP
