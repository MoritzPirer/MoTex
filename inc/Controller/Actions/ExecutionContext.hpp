///
/// @file: ExecutionContext.hpp
/// @description: description
///
/// @date: 2026-02-16
/// @author: Moritz Pirer
///

#ifndef EXECUTION_CONTEXT_HPP
#define EXECUTION_CONTEXT_HPP

class EditorState;
class UndoRedoManager;

struct ExecutionContext {
    EditorState& state;
    UndoRedoManager& UndoRedoManager;
};

#endif //EXECUTION_CONTEXT_HPP
