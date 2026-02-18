#include "../../../../inc/Controller/IO/FileHandler.hpp"
#include "../../../../inc/Controller/IO/FileException.hpp"

#include "../../../../inc/Controller/Actions/System/SaveAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

SaveAction::SaveAction(SaveConfirmation confirm_save):
    m_confirm_save{confirm_save} {}

void SaveAction::apply(ExecutionContext& context) {
    try {
        FileHandler::saveFile(context.state.getFile());

        if (m_confirm_save == SaveConfirmation::YES) {
            context.state.addTemporaryMessage("Saved all changes!");
        }
    }
    catch (FileException& e) {
        context.state.addTemporaryMessage("Saving failed!");
    }

}