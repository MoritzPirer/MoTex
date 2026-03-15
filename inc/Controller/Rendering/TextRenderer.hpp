///
/// @file: TextRenderer.hpp
/// @description: description
///
/// @date: 2026-3-15
/// @author: Moritz Pirer
///

#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include <vector>

#include "../../Shared/DataFlow/VisualSegment.hpp"
#include "../../Model/EditorState.hpp"
#include "EscapeState.hpp"

class TextRenderer {
private:
    const EditorState& m_state; 

    const char c_underline_indicator = '-';
    const char c_min_underline_count = 2;
    const char c_heading_indicator = '#';
    const char c_quote_indicator = '>';
    const char c_textstyle_modifier = '*';
    const char c_style_change_blocker = '`';
    const char c_escape_char = '\\';

    struct RenderState {
        std::vector<VisualSegment>& segments;
        std::string current_chunk;
        TextStyle& style;
        EscapeState& escape_state;
        TextRole text_role;

        void commit();        
        void add(char c);
    };

    std::vector<std::vector<VisualSegment>> renderHighlights(std::vector<std::string> split_paragraph, int max_width,
    int current_paragraph, int visual_rows_available, int first_visible);

    TextRole getTextRole(int current_paragraph);
    bool isHeading(int paragraph_index);
    bool isFollowedByUnderline(int paragraph_index);
    bool isQuote(int paragraph_index);

    std::pair<EscapeState, TextStyle> parseOffscreenParagraphs(int current_paragraph, int first_visible);

    std::vector<VisualSegment> renderScreenRow(const std::string& line, TextStyle& style, TextRole text_role, EscapeState& escape_state);

    bool handleSpecialCharacters(char current, RenderState& render_state);

    void parseStyleModifier(const std::string& line, size_t& index, RenderState& render_state);

public:
    TextRenderer(const EditorState& state);
    TextRenderer(const TextRenderer&) = default;
    ~TextRenderer() = default;

    std::vector<std::vector<VisualSegment>> renderVisibleText(ScreenSize text_area_size);
};

#endif //TEXT_RENDERER_HPP
