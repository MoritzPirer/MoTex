///
/// @file: MoveToAction.hpp
/// @description: describes an action that moves the cursor to a given position
///
/// @date: 2026-02-06
/// @author: Moritz Pirer
///

#ifndef MOVE_TO_ACTION
#define MOVE_TO_ACTION

#include "SizeDependantAction.hpp"
#include "../../Shared/Direction.hpp"

class MoveToAction: public Action {
private:
    ScreenSize m_text_area_size;
    Position m_target_position;
public:
    MoveToAction(ScreenSize text_area_size, Position target_position);
    MoveToAction(const MoveToAction&) = default;
    ~MoveToAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //MOVE_TO_ACTION
