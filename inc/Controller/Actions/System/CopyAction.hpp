///
/// @file: CopyAction.hpp
/// @description: An action that copies some text to the clipboard
///
/// @date: 2026-02-24
/// @author: Moritz Pirer
///

#ifndef COPY_ACTION_HPP
#define COPY_ACTION_HPP

#include "../SpanAction.hpp"

class CopyAction: public SpanAction {
private:
    CopyType m_type;
public:
    CopyAction(Position start, Position end, CopyType type);
    CopyAction(const CopyAction&) = default;
    ~CopyAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //COPY_ACTION_HPP
