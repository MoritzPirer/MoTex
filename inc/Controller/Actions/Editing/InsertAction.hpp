///
/// @file: InsertAction.hpp
/// @description: Inserts some text at a given position
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef INSERT_ACTION_HPP
#define INSERT_ACTION_HPP

#include <vector>

#include "../Action.hpp"

class InsertAction: public Action {
private:
    std::vector<std::string> m_content;
    Position m_start;
    bool m_supress_merge;
public:
    InsertAction(std::vector<std::string> content, Position start, bool supress_merge = false);
    InsertAction(const InsertAction&) = default;
    ~InsertAction() = default;

    void apply(ExecutionContext& context) override;

    bool canBeUndone() const override;

    void undo(EditorState& state) override;

    bool canAbsorb(const std::shared_ptr<Action>& action) const override;

    void absorb(const std::shared_ptr<Action>& action) override;
};

#endif //INSERT_ACTION_HPP
