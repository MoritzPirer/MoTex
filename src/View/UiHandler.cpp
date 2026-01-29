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

void UiHandler::renderModeLabel(const RenderInfo& render_info) {
    m_display_handler.renderModeLabel(render_info.getModeLabel());
}

ScreenSize UiHandler::screenSize() const {
    return m_display_handler.screenSize();
}

void UiHandler::render(const RenderInfo& render_info) {
    clear();

    renderText(render_info);
    renderModeLabel(render_info);
    renderCursor(render_info);

    refresh();
}

int UiHandler::getInput() {
    int raw_input = m_input_handler.getInput();
    return m_input_handler.translateInput(raw_input);
}