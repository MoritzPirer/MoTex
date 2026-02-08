///
/// @file: ActionDirection.hpp
/// @description: In which direction an action should be performed
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef ACTION_DIRECTION_HPP
#define ACTION_DIRECTION_HPP

#include "../../../Shared/Direction.hpp"

enum class ActionDirection {
    FORWARD,
    BACKWARD
};

inline Direction toDirection(ActionDirection direction) {
    switch (direction) {
        case ActionDirection::FORWARD:
            return Direction::RIGHT;
        case ActionDirection::BACKWARD:
            return Direction::LEFT;
    }
}

inline ActionDirection getOppositeDirection(ActionDirection direction) {
    switch (direction) {
        case ActionDirection::FORWARD:
            return ActionDirection::BACKWARD;
        case ActionDirection::BACKWARD:
            return ActionDirection::FORWARD;
    }
}

#endif //ACTION_DIRECTION_HPP
