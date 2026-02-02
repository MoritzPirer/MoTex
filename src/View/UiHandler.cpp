#include <ncurses.h>

#include "../../inc/View/UiHandler.hpp"

void UiHandler::renderText(const RenderInfo& render_info) {
    for (int i = 0; i < render_info.getRowCount(); i++) {
        m_display_handler.renderLine(i, render_info.getVisualRow(i));
    }
}

void UiHandler::renderCursor(const RenderInfo& render_info) {
    m_display_handler.renderCursor(render_info.getCursorPosition());
}

void UiHandler::renderMetadata(const RenderInfo& render_info) {
    int metadata_offset = render_info.getRowCount();
    for (int i = 0; i < render_info.getMetadataRowCount(); i++) {
        m_display_handler.renderLine(i + metadata_offset, render_info.getMetadataRow(i));
    }
}

ScreenSize UiHandler::screenSize() const {
    return m_display_handler.screenSize();
}

void UiHandler::render(const RenderInfo& render_info) {
    clear();

    renderText(render_info);
    renderMetadata(render_info);
    renderCursor(render_info);

    refresh();
}

int UiHandler::getInput() {
    int raw_input = m_input_handler.getInput();
    return m_input_handler.translateInput(raw_input);
}