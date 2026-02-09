#include "../../inc/Shared/Direction.hpp"

Direction getOppositeDirection(Direction direction) {
    switch (direction) {
    case Direction::RIGHT: {
        return Direction::LEFT; 
    }

    case Direction::LEFT: {
        return Direction::RIGHT;
    }

    case Direction::UP: {
        return Direction::DOWN;
    }

    case Direction::DOWN: {
        return Direction::UP;
    }

    default: {
        break;
    }

    }
}