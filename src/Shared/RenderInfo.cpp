#include "../../inc/Shared/RenderInfo.hpp"

RenderInfo::RenderInfo(std::vector<std::string> visual_rows,
        std::string mode_label, Position cursor_position):
    m_visual_rows{visual_rows},
    m_mode_label{mode_label},
    m_cursor_position{cursor_position} {}

const std::string& RenderInfo::getVisualRow(int index) const { return m_visual_rows.at(index); }

int RenderInfo::getRowCount() const { return m_visual_rows.size(); }

const std::string& RenderInfo::getModeLabel() const { return m_mode_label; }

const Position& RenderInfo::getCursorPosition() const { return m_cursor_position; }