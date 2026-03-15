#include <algorithm>

#include "../../../inc/Controller/Rendering/TextRenderer.hpp"
#include "../../../inc/Shared/Utils/StringHelpers.hpp"

using std::vector, std::string;

TextRenderer::TextRenderer(const EditorState& state): m_state{state} {}

void TextRenderer::RenderState::commit() {
    if (!current_chunk.empty()) {
        segments.push_back({current_chunk, text_role, style});
        current_chunk.clear();
    }
}

void TextRenderer::RenderState::add(char c) {
    current_chunk += c;
}

vector<vector<VisualSegment>> TextRenderer::renderVisibleText(ScreenSize text_area_size) {
    Position first_visible = m_state.getFirstVisibleChar(text_area_size);

    int current_paragraph = first_visible.row;
    vector<vector<VisualSegment>> visible_rows;    
    visible_rows.reserve(text_area_size.height);


    bool is_first_paragraph = true;
    for (int visual_row = 0; visual_row < text_area_size.height;) {

        if (static_cast<size_t>(current_paragraph) >= m_state.getNumberOfParagrahps()) { 
            visible_rows.push_back({VisualSegment{
                "~",
                TextRole::WEAK_HIGHLIGHT,
                TextStyle::makeNormal()
            }}); // FUTURE: load placeholder line from settings

            visual_row++;
            continue;
        }

        int start_column = (is_first_paragraph? first_visible.column : 0);
        is_first_paragraph = false;

        vector<string> split = StringHelpers::splitIntoRows(
            m_state.getParagraph(current_paragraph),
            start_column,
            text_area_size.width 
        );
        

        vector<vector<VisualSegment>> temp;
        temp = renderHighlights(split,
            text_area_size.width,
            current_paragraph,
            text_area_size.height - visual_row,
            start_column
        );

        visible_rows.insert(visible_rows.end(), temp.begin(), temp.end());
        visual_row += split.size();
        current_paragraph++;
    }
    
    return visible_rows;
}

vector<vector<VisualSegment>> TextRenderer::renderHighlights(vector<string> split_paragraph, int max_width,
    int current_paragraph, int visual_rows_available, int first_visible) {

    TextRole text_role = TextRole::NORMAL_TEXT;
    if (m_state.getFileName().ends_with(".md")) {
        text_role = getTextRole(current_paragraph);
    }

    if (text_role == TextRole::STRONG_HIGHLIGHT) {
        for (string& line : split_paragraph) {
            line = StringHelpers::leftAlign(line, max_width);
        }
    }

    auto [escape_state, style] = parseOffscreenParagraphs(current_paragraph, first_visible);
    // escape_state.has_read_partial_modifier = false;

    vector<vector<VisualSegment>> segments;

    for (const string& line : split_paragraph) {
        if (visual_rows_available <= 0) {
            break;
        }

        segments.push_back(renderScreenRow(line, style, text_role, escape_state));
        visual_rows_available--;
    }

    return segments;
}

TextRole TextRenderer::getTextRole(int current_paragraph) {
    if (isHeading(current_paragraph)) {
        return TextRole::WEAK_HIGHLIGHT;
    }

    if (isQuote(current_paragraph)) {
        return TextRole::STRONG_HIGHLIGHT;
    }

    return TextRole::NORMAL_TEXT;
}

bool TextRenderer::isHeading(int paragraph_index) {
    return isFollowedByUnderline(paragraph_index)
        || StringHelpers::startsWith(m_state.getParagraph(paragraph_index), c_heading_indicator);
}

bool TextRenderer::isFollowedByUnderline(int paragraph_index) {
    if (m_state.isLastParagraph(paragraph_index)) {
        return false;
    }

    const string& next = m_state.getParagraph(paragraph_index + 1);

    return (StringHelpers::consistsOnlyOfIgnoringWhitespace(next, c_underline_indicator)
        && std::ranges::count(next, c_underline_indicator) >= c_min_underline_count);
}


bool TextRenderer::isQuote(int paragraph_index) {
    return StringHelpers::startsWithIgnoringWhitespace(m_state.getParagraph(paragraph_index), c_quote_indicator);
}


std::pair<EscapeState, TextStyle> TextRenderer::parseOffscreenParagraphs(int current_paragraph, int first_visible) {
    EscapeState escape_state = {
        .is_style_change_disabled = false,
        .has_read_partial_modifier = false,
        .has_read_escape_char = false
    };

    TextStyle style = TextStyle::makeNormal();
    const string& paragraph = m_state.getParagraph(current_paragraph);

    for (int i = 0; i < first_visible && static_cast<size_t>(i) < paragraph.length(); i++) {
        if (paragraph.at(i) == c_style_change_blocker) {
            escape_state.is_style_change_disabled = !escape_state.is_style_change_disabled;
            continue;
        }

        if (escape_state.is_style_change_disabled) {
            continue;
        }
        
        if (paragraph.at(i) == c_escape_char) {
            i++;
            continue;
        }
        
        if (paragraph.at(i) == c_textstyle_modifier) {
            if (static_cast<size_t>(i) + 1 < paragraph.length() && paragraph.at(i + 1) == c_textstyle_modifier) {
                style.toggleBold();
                i++; //consume second modifier
                escape_state.has_read_partial_modifier = false;
            }
            else {
                style.toggleItalic();
                escape_state.has_read_partial_modifier = true;
            }
        }
    }

    return {escape_state, style};
}

vector<VisualSegment> TextRenderer::renderScreenRow(const string& line, TextStyle& style, TextRole text_role, EscapeState& escape_state) {
    vector<VisualSegment> line_segments;

    RenderState render_state = {
        .segments = line_segments,
        .current_chunk = "",
        .style = style,
        .escape_state = escape_state,
        .text_role = text_role
    };

    for (size_t i = 0; i < line.length(); ++i) {
        char current = line.at(i);

        if (handleSpecialCharacters(current, render_state)) {
            continue;
        }

        render_state.commit();
        
        bool is_modifier = (current == c_textstyle_modifier);
        render_state.add(current);
        
        if (is_modifier) {
            parseStyleModifier(line, i, render_state);
        }
    }

    render_state.commit();
    return render_state.segments;
}

bool TextRenderer::handleSpecialCharacters(char current, RenderState& render_state) {
    if (render_state.escape_state.has_read_escape_char) {
        render_state.escape_state.has_read_escape_char = false;
        render_state.add(current);
        return true;
    }

    if (current == c_style_change_blocker) {
        render_state.escape_state.is_style_change_disabled = !render_state.escape_state.is_style_change_disabled;
        render_state.add(current);
        return true;
    }

    if (render_state.escape_state.is_style_change_disabled) {
        render_state.add(current);
        return true;
    }

    if (current == c_escape_char) {
        render_state.escape_state.has_read_escape_char = true;
        render_state.add(current);
        return true;
    }

    return false;
}

void TextRenderer::parseStyleModifier(const string& line, size_t& index, RenderState& render_state) {
    if ((index == 0 && render_state.escape_state.has_read_partial_modifier)
        || (index + 1 < line.length() && line.at(index + 1) == c_textstyle_modifier)) { 

        index++;
        render_state.escape_state.has_read_partial_modifier = false;
        render_state.style.toggleBold();
        render_state.add(line.at(index));
    }
    else if (index + 1 < line.length() && line.at(index + 1) != c_textstyle_modifier) {
        render_state.style.toggleItalic(); 
        render_state.escape_state.has_read_partial_modifier = false;
    }
    else {
        render_state.escape_state.has_read_partial_modifier = true;
    }

}