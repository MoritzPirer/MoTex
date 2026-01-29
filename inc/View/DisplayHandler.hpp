///
/// @file: DisplayHandler.hpp
/// @description: Responsible for handeling everything related to displaying text to the user
///
/// @date: 2026-01-28
/// @author: Moritz Pirer
///

#ifndef DISPLAY_HANDLER_HPP
#define DISPLAY_HANDLER_HPP

#include <string>

#include "../Shared/ScreenSize.hpp"
#include "../Shared/Position.hpp"

class DisplayHandler {
public:
    DisplayHandler() = default;
    DisplayHandler(const DisplayHandler&) = default;
    ~DisplayHandler() = default;

    /// @brief returns the current size of the screen in characters wide and rows tall    
    ScreenSize screenSize() const;

    /// @brief draws the cursor to the screen
    /// @param screen_position the position to draw the cursor 
    void renderCursor(Position screen_position);

    /// @brief draws a line of text to the screen
    /// @param start_visual_row what row to start drawing. Gets updated to reflect the new
    ///     starting position after rendering this paragraph
    /// @param line the line to render
    void renderLine(int& start_visual_row, const std::string& line);

    void renderModeLabel(const std::string& label);
};

#endif //DISPLAY_HANDLER_HPP
