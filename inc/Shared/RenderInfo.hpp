///
/// @file: RenderInfo.hpp
/// @description: the data that should be rendered
///
/// @date: 2026-01-29
/// @author: Moritz Pirer
///

#ifndef RENDER_INFO_HPP
#define RENDER_INFO_HPP

#include <vector>
#include <string>

#include "Position.hpp"

class RenderInfo {
private:
    std::vector<std::string> m_visual_rows;
    std::string m_mode_label;
    Position m_cursor_position;

public:
    RenderInfo(std::vector<std::string> visual_rows,
        std::string mode_label, Position cursor_position);

    RenderInfo(const RenderInfo&) = default;
    ~RenderInfo() = default;

    const std::string& getVisualRow(int index) const;
    int getRowCount() const;
    const std::string& getModeLabel() const;
    const Position& getCursorPosition() const;
};

#endif //RENDER_INFO_HPP
