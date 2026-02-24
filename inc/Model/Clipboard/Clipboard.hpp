///
/// @file: Clipboard.hpp
/// @description: holds copied content
///
/// @date: 2024-02-24
/// @author: Moritz Pirer
///

#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP

#include <vector>
#include <string>

#include "CopyType.hpp"

struct Clipboard {
    CopyType type;
    std::vector<std::string> content;
};

#endif //CLIPBOARD_HPP
