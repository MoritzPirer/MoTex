///
/// @file: SpanMoveAction.hpp
/// @description: moves backwards or forwards until a set of delimiters is reached or passed
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef SPAN_MOVE_ACTION
#define SPAN_MOVE_ACTION

#include "../SpanAction.hpp"

class SpanMoveAction: public SpanAction {
private:
    Direction m_direction;
public:
    SpanMoveAction(Position start, Position end, Direction direction);
    SpanMoveAction(const SpanMoveAction&) = default;
    ~SpanMoveAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //SPAN_MOVE_ACTION
