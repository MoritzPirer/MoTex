///
/// @file: EditorMode.hpp
/// @description: abstract template for a mode the editor can be in
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef EDITOR_MODE_HPP
#define EDITOR_MODE_HPP

#include <string>
#include <memory>
#include <vector>

#include "../Action/Action.hpp"
#include "../../Shared/ParseResult.hpp"
#include "../Settings/Settings.hpp"
#include "../../Shared/Input.hpp"
#include "../Control/ParsingContext.hpp"

class EditorMode {
public:
    EditorMode() = default;
    EditorMode(const EditorMode&) = default;
    virtual ~EditorMode() = default;

    virtual ParseResult parseInput(Input input, ParsingContext context) = 0;

    /// @brief returns the name of the mode for displaying
    virtual std::string getModeLabel() const = 0;
};

#endif //EDITOR_MODE_HPP
