#include "../../../inc/Controller/Control/EditorController.hpp"
#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Shared/StringHelpers.hpp"

EditorController::EditorController(std::optional<std::string> file_path):
    m_mode_manager{ModeType::TOOL_MODE} {

    TextFile file;
    if (!file_path.has_value() || file_path->empty()) {
        std::filesystem::path name = FileHandler::getDefaultName(); 
        file = FileHandler::createFile(name);
    }
    else {
        file = FileHandler::openFile(file_path.value());      
    }
    m_state = EditorState(file);
}

Position EditorController::getFirstVisibleChar(ScreenSize size) {
    return m_state.getFirstVisibleChar(size);
}

std::vector<std::string> EditorController::splitIntoRows(const std::string& paragraph,
    int start_column, int max_length) const {

    if (paragraph.length() == 0) {
        return {""};
    }

    std::vector<std::string> split;
    //split.reserve((paragraph.length() - start_column) / width);

    for (int i = start_column; static_cast<size_t>(i) < paragraph.length(); i += max_length) {
        split.emplace_back(paragraph.substr(i, max_length));
    }

    return split;
}

std::vector<std::string> EditorController::calculateVisibleRows(ScreenSize text_area_size) {
    Position first_visible = getFirstVisibleChar(text_area_size);

    int current_paragraph = first_visible.row;
    std::vector<std::string> visible_rows;    
    visible_rows.reserve(text_area_size.height);

    bool is_first_paragraph = true;
    for (int visual_row = 0; visual_row < text_area_size.height;) {

        if (static_cast<size_t>(current_paragraph) < m_state.getNumberOfParagrahps()) { 
            
            std::vector<std::string> split = splitIntoRows(
                m_state.getParagraph(current_paragraph),
                (is_first_paragraph? first_visible.column : 0),
                text_area_size.width 
            );

            is_first_paragraph = false;


            for (const std::string& line : split) {
                if (visual_row < text_area_size.height) {
                    visible_rows.emplace_back(line);
                }

                visual_row++;
            }
        
            current_paragraph++;
        }        
        else {
            visible_rows.emplace_back("~"); // FUTURE: load placeholder line from settings
            visual_row++;
        }
    }
    
    return visible_rows;
}

Position EditorController::calculateScreenPositionOfCursor(ScreenSize text_area_size) {
    int screen_row = std::min(
        text_area_size.height / 2,
        m_state.calculateVisualLineOfCursor(text_area_size.width)
    ); 

    int screen_column = m_state.getCursor().getColumn() % text_area_size.width;

    return {screen_row, screen_column};
}

std::vector<std::string> EditorController::calculateFileContentInfo(ScreenSize actual_size) {
    bool show_character_count = true;
    bool show_word_count = true;
    bool show_paragraph_count = true;
    bool show_cursor_position = true;

    std::vector<std::string> parts;

    if (show_character_count) {
        int character_count = m_state.getNumberOfCharacters();
        parts.push_back(std::to_string(character_count) + " character(s)");
    }

    if (show_word_count) {
        int word_count = m_state.getNumberOfWords();
        parts.push_back(std::to_string(word_count) + " word(s)");
    }

    if (show_paragraph_count) {
        parts.push_back(std::to_string(m_state.getNumberOfParagrahps()) + " paragraph(s)");
    }

    if (show_cursor_position) {
        parts.push_back("Cursor is in line / column: " +
            m_state.getCursor().getPosition().format());
    }
    
    std::string file_content_info = StringHelpers::join(parts, " | "); 
    return splitIntoRows(file_content_info, 0, actual_size.width);
}

std::string EditorController::getSaveStateIcon() {
    switch (m_state.getSaveState()) {
        case SaveState::NEVER_SAVED: return "!";
        case SaveState::UNSAVED_CHANGES: return "*";
        case SaveState::SAVED: return "=";
        default:
            throw std::logic_error("Save state had an unknown value!");
    }
}

std::string EditorController::getSaveStateDescription() {
    switch (m_state.getSaveState()) {
        case SaveState::NEVER_SAVED: return "Never saved";
        case SaveState::UNSAVED_CHANGES: return "Unsaved changes";
        case SaveState::SAVED: return "All changes saved";
        default:
            throw std::logic_error("Save state had an unknown value!");
    }
}

void EditorController::addEditorVersionTo(std::string& metadata_line, ScreenSize actual_size) {
    bool show_version = true;
    if (!show_version) {
        return;
    }

    const size_t min_padding = 5;

    std::string version_string = getVersion();
    int space_for_version = actual_size.width - static_cast<int>(metadata_line.length());
    
    //more space for version needed
    if (static_cast<size_t>(space_for_version) < min_padding + version_string.length()
        || space_for_version < 0) {
            
        int needed_space = (actual_size.width - metadata_line.length() % actual_size.width);
        metadata_line += std::string(needed_space, ' ');
        version_string = StringHelpers::rightAlign(version_string, actual_size.width); 
    }
    // enough space, place version at right end
    else {
        metadata_line =
            StringHelpers::leftAlign(metadata_line, actual_size.width - version_string.length());
    }

    metadata_line += version_string;
}

std::vector<std::string> EditorController::calculateFileStatusInfo(ScreenSize actual_size) {
    bool show_editor_mode = true;
    bool show_file_name = true; 
    bool show_save_state_icon = true;
    bool show_save_state_desctiption = true;
    bool show_version = true;

    std::vector<std::string> parts;
    if (show_editor_mode) {
        parts.push_back(m_mode_manager.getModeLabel());
    }

    if (show_file_name) {
        parts.push_back(m_state.getFileName());
    }

    if (show_save_state_icon) {
        std::string part = "[" + getSaveStateIcon() + "]";

        if (show_save_state_desctiption) {
            part += " " + getSaveStateDescription();
        }

        parts.push_back(part);
    }

    if (show_version) {
        parts.push_back(getVersion());
    }

    std::string file_status_info = StringHelpers::join(parts, " | ");

    return splitIntoRows(file_status_info, 0, actual_size.width);
}

std::vector<std::string> EditorController::calculateMetadataRows(ScreenSize actual_size) {
    bool show_seperator = true;

    std::vector<std::string> result;

    result.emplace_back(actual_size.width, (show_seperator? '-' : ' '));
    
    auto addContent = [&](const std::vector<std::string>& rows) {
        result.insert(result.end(), rows.begin(), rows.end());
        result.push_back("");
    };

    addContent(calculateFileContentInfo(actual_size));
    addContent(calculateFileStatusInfo(actual_size));

    return result;
}

RenderInfo EditorController::calculateRenderInfo(ScreenSize actual_size) {
    std::vector<std::string> metadata_rows = calculateMetadataRows(actual_size);
    ScreenSize text_area_size = actual_size; //FUTURE: also account for line number width
    text_area_size.height -= metadata_rows.size();

    return {
        calculateVisibleRows(text_area_size),
        calculateMetadataRows(actual_size),
        calculateScreenPositionOfCursor(text_area_size),
    };
}

void EditorController::mainLoop() {
    while (m_state.getIsQuit() == false) {
        RenderInfo render_info = calculateRenderInfo(m_ui_handler.screenSize());
        m_ui_handler.render(render_info);
        int input = m_ui_handler.getInput();

        std::vector<std::shared_ptr<Action>> actions = m_mode_manager.convertToAction(
            input,
            m_ui_handler.screenSize()
        );

        for (std::shared_ptr action : actions) {
            action->applyTo(m_state);
        } 
    }
}