#include "../../../../inc/Controller/Actions/System/CopyAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

CopyAction::CopyAction(Position start, Position end, CopyType type):
    SpanAction(start, end),
    m_type{type}
    {}

void CopyAction::apply(ExecutionContext& context) {
    Clipboard clipboard = {
        m_type,
        context.state.copyRange(m_start, m_end)
    };

    context.state.setClipboard(clipboard);
}