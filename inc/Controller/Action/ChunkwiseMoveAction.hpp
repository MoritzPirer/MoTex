///
/// @file: ChunkwiseMoveAction.hpp
/// @description: Move the character a by a chunk of text at a time
///
/// @date: 2026-01-30
/// @author: Moritz Pirer
///

#ifndef CHUNKWISE_MOVE_ACTION_HPP
#define CHUNKWISE_MOVE_ACTION_HPP

#include "Action.hpp"
#include "../../Shared/Destination.hpp"
#include "../../Shared/Scope.hpp"

class ChunkwiseMoveAction: public Action {
private:
    const Scope m_scope;
    const Destination m_destination;

    void fileScopeMove(EditorState& state);
    void paragraphScopeMove(EditorState& state);
    void phraseScopeMove(EditorState& state);
    void wordScopeMove(EditorState& state);
public:
    ChunkwiseMoveAction(Scope scope, Destination destination);
    ChunkwiseMoveAction(const ChunkwiseMoveAction&) = default;
    ~ChunkwiseMoveAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //CHUNKWISE_MOVE_ACTION_HPP
