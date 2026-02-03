///
/// @file: SaveState.hpp
/// @description: describes if a file has unsaved changes
///
/// @date: 2026-02-02
/// @author: Moritz Pirer
///

#ifndef SAVE_STATE_HPP
#define SAVE_STATE_HPP

enum class SaveState {
    NEW_FILE,
    UNSAVED_CHANGES,
    SAVED
};

#endif //SAVE_STATE_HPP
