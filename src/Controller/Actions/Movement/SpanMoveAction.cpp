#include "../../../../inc/Controller/Actions/Movement/SpanMoveAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

SpanMoveAction::SpanMoveAction(Position start, Position end, Direction direction):
    SpanAction{start, end},
    m_direction{direction}
    {}

void SpanMoveAction::apply(ExecutionContext& context) {
    if (m_direction == Direction::LEFT) {
        context.state.moveCursorTo(m_start);
    }
    else {
        context.state.moveCursorTo(m_end);
    }
}