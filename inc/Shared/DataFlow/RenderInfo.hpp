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

#include "../Types/Position.hpp"
#include "VisualSegment.hpp"

class RenderInfo {
private:
    std::vector<std::vector<VisualSegment>> m_text_area_rows;

    std::vector<std::vector<VisualSegment>> m_panel_rows;
    std::vector<VisualSegment> m_aside_rows;
    int m_aside_width;
    std::vector<VisualSegment> m_overlay_rows;
    Position m_cursor_position;
    
    bool m_render_colors;

public:
    RenderInfo(
        std::vector<std::vector<VisualSegment>> text_area_rows,
        std::vector<std::vector<VisualSegment>> panel_rows,
        std::vector<VisualSegment> aside_rows,
        int aside_width,
        std::vector<VisualSegment> overlay_rows,
        Position cursor_position,
        bool render_in_color
    );

    RenderInfo(const RenderInfo&) = default;
    ~RenderInfo() = default;

    const std::vector<VisualSegment>& getTextAreaRow(int index) const;
    int getTextAreaRowCount() const;
    
    const std::vector<VisualSegment>& getPanelRow(int index) const;
    int getPanelRowCount() const;

    VisualSegment getAsideRow(int index) const;
    int getAsideRowCount() const;
    int getAsideWidth() const;

    VisualSegment getOverlayRow(int index) const;
    int getOverlayRowCount() const;

    bool shouldRenderColors() const;

    const Position& getCursorPosition() const;
};

#endif //RENDER_INFO_HPP
