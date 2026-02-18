///
/// @file: QuitMode.hpp
/// @description: The way in which the editor should be quit
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef QUIT_MODE_HPP
#define QUIT_MODE_HPP

enum class QuitMode {
    ONLY_IF_SAVED,
    FORCE_QUIT
};

#endif //QUIT_MODE_HPP
