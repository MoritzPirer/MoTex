///
/// @file: QuitAction.hpp
/// @description: quits the editor
///
/// @date: 2026-01-26
/// @author: Moritz Pirer
///

#ifndef QUIT_ACTION_HPP
#define QUIT_ACTION_HPP

#include "Action.hpp"

class QuitAction: public Action {
private:

public:
    QuitAction() = default;
    QuitAction(const QuitAction&) = default;
    ~QuitAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //QUIT_ACTION_HPP
