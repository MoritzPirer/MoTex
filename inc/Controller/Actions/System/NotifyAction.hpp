///
/// @file: NotifyAction.hpp
/// @description: describes an action that moves the cursor to a given position
///
/// @date: 2026-02-06
/// @author: Moritz Pirer
///

#ifndef NOTIFY_ACTION_HPP
#define NOTIFY_ACTION_HPP

#include "../Action.hpp"

class NotifyAction: public Action {
private:
    std::string m_message;
public:
    NotifyAction(std::string message);
    NotifyAction(const NotifyAction&) = default;
    ~NotifyAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //NOTIFY_ACTION_HPP
