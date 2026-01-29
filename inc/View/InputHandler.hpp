///
/// @file: InputHandler.hpp
/// @description: handles everything related to receiving input from the user
///
/// @date: 2026-01-28
/// @author: Moritz Pirer
///

#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

class InputHandler {
public:
    InputHandler() = default;
    InputHandler(const InputHandler&) = default;
    ~InputHandler() = default;

    /// @brief waits for the user to input a key and then returns it
    int getInput();

    /// @brief translates special keys (e.g. escape, backspace) to the internal
    ///     standard defined in Shared/SpecialInputs.hpp. Normal keys are unaffected
    int translateInput(int original_input);
};

#endif //INPUT_HANDLER_HPP
