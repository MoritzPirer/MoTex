#include <ncurses.h>
#include "../../../inc/Controller/Control/EditorController.hpp"
#include "../../../inc/Controller/Control/FileHandler.hpp"

EditorController::EditorController(std::optional<std::string> file_path):
    m_mode_manager{ModeType::TOOL_MODE} {

    FileHandler file_handler;
    TextFile file;
    if (!file_path.has_value() || file_path->empty()) {
        std::filesystem::path name = file_handler.getDefaultName(); 
        file = file_handler.createFile(name);
    }
    else {
        file = file_handler.openFile(file_path.value());      
    }
    m_state = EditorState(file);
}

std::vector<std::string> EditorController::splitIntoRows(const std::string& paragraph, int start_column) const {
    if (paragraph.length() == 0) {
        return {""};
    }

    int width = m_ui_handler.screenSize().width;
    std::vector<std::string> split;
    split.reserve((paragraph.length() - start_column) / width);

    for (int i = start_column; static_cast<size_t>(i) < paragraph.length(); i += width) {
        split.emplace_back(paragraph.substr(i, width));
    }

    return split;
}

RenderInfo EditorController::calculateRenderInfo() {
    const ScreenSize size = m_ui_handler.screenSize();
    Position first_visible = getFirstVisibleChar(size);

    int current_paragraph = first_visible.row;
    std::vector<std::string> visible_rows;    
    visible_rows.reserve(size.height);

    for (int visual_row = 0; visual_row < size.height; /*visual_row++*/) {

        if (static_cast<size_t>(current_paragraph) < m_state.getLineCount()) { 
            
            std::vector<std::string> split = splitIntoRows(m_state.getLine(current_paragraph), first_visible.column);
            first_visible.column = 0;

            for (const std::string& line : split) {
                visible_rows.emplace_back(line);
                visual_row++;
            }
        
            current_paragraph++;
        }        
        else {
            visible_rows.emplace_back("~");
            visual_row++;
        }
    }

    Position cursor_screen_position = {screenLineOfCursor(size), m_state.getCursor().getColumn() % size.width};
    return {
        visible_rows,
        m_mode_manager.getModeLabel(),
        cursor_screen_position
    };
    
}

int EditorController::getCursorRow() const {
    return m_state.getCursor().getPosition().row;
}

int EditorController::getCursorColumn() const {
    return m_state.getCursor().getPosition().column;
}

std::string EditorController::getModeLabel() const {
    return m_mode_manager.getModeLabel();
}

void EditorController::processInput(int input, ScreenSize size) {
    std::vector<std::shared_ptr<Action>> actions = m_mode_manager.convertToAction(input, size);
    for (std::shared_ptr action : actions) {
        action->applyTo(m_state);
    }
}

Position EditorController::getFirstVisibleChar(ScreenSize size) {
    return m_state.getFirstVisibleChar(size);
}

bool EditorController::isQuit() const {
    return m_state.getIsQuit();
}

int EditorController::getLineCount() const {
    return m_state.getLineCount();
}

std::string EditorController::getPartialLine(Position start) {
    return m_state.getPartialLine(start);
}

const std::string& EditorController::getLine(size_t row) const {
    return m_state.getLine(row);
}

int EditorController::screenLineOfCursor(ScreenSize size) {
    int visual_line = m_state.calculateVisualLineOfCursor(size.width);


    return std::min(size.height / 2, visual_line);
}

void EditorController::mainLoop() {
    while (m_state.getIsQuit() == false) {
        RenderInfo render_info = calculateRenderInfo();
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