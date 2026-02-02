///
/// @file: UiHandler.hpp
/// @description: Handle the interaction with the user
///
/// @date: 2026-01-28
/// @author: Moritz Pirer
///

#ifndef UI_HANDLER_HPP
#define UI_HANDLER_HPP

#include "NcursesSession.hpp"
#include "InputHandler.hpp"
#include "DisplayHandler.hpp"

#include "../Shared/ScreenSize.hpp"
#include "../Shared/RenderInfo.hpp"

class UiHandler {
private:
    NcursesSession m_ncurses_session;
    InputHandler m_input_handler;
    DisplayHandler m_display_handler;

    void renderText(const RenderInfo& render_info);
    void renderCursor(const RenderInfo& render_info);
    void renderMetadata(const RenderInfo& render_info);
public:
    UiHandler() = default;
    UiHandler(const UiHandler&) = delete;
    ~UiHandler() = default;

    ScreenSize screenSize() const;
    void render(const RenderInfo& render_info);
    int getInput();
};

#endif //UI_HANDLER_HPP
