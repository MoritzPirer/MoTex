///
/// @file: EditorState.hpp
/// @description: description
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include "../../inc/Model/TextFile.hpp"
#include "../../inc/Model/Cursor.hpp"
#include "../../inc/Controller/ScreenSize.hpp"

class EditorState {
private:
    TextFile m_file;
    Cursor m_cursor;
    bool m_is_quit;

    bool isLastVisualLineOfLine(int screen_width);
    
    Position skipOffscreenLines(int offscreen_visual_lines, int screen_width) const;

public:
    EditorState():
        m_file{},
        m_cursor{},  
        m_is_quit{false} {}

    EditorState(TextFile file): m_file{file} {}  
    
    EditorState(const EditorState&) = default;
    ~EditorState() = default;
    
    const Cursor& getCursor() const { return m_cursor; }
    TextFile& getFile() { return m_file; }
    bool getIsQuit() const { return m_is_quit; }
    void setIsQuit(bool is_quit) { m_is_quit = is_quit; } 
    
    void moveCursorUp(int screen_width);
    void moveCursorLeft();
    void moveCursorDown(int screen_width);
    void moveCursorRight();

    void insertCharacter(char character_to_add);
    void deleteRange(Position start, Position end);
    void splitAtCursor();
    void joinLineToPrevious(int line);

    
    Position getFirstVisibleChar(ScreenSize size);
    
    size_t getLineCount() const { return m_file.getLineCount(); }
    
    std::string getPartialLine(Position start);
    const std::string& getLine(size_t row) const { return m_file.getLine(row); }
    int calculateVisualLineOfCursor(int screen_width) const;
};

#endif //EDITOR_STATE_HPP
