#include "../../../../inc/Controller/Actions/System/NotifyAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

NotifyAction::NotifyAction(std::string message):
    m_message{message} {}

void NotifyAction::apply(ExecutionContext& context) {
    context.state.addTemporaryMessage(m_message);
}