#include <unordered_map>
#include <regex>

#include "../../../inc/Controller/Modes/TypingMode.hpp"
#include "../../../inc/Controller/Actions/Editing/InsertAction.hpp"
#include "../../../inc/Controller/Actions/Editing/DeleteAction.hpp"
#include "../../../inc/Controller/Actions/Structure/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Actions/Movement/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Actions/Editing/IndentAction.hpp"
#include "../../../inc/Controller/Actions/Editing/UnindentAction.hpp"

#include "../../../inc/Shared/Types/SpecialKey.hpp"
#include "../../../inc/Shared/Utils/StringHelpers.hpp"


using std::make_shared;

ParseResult TypingMode::parseSpecialKey(SpecialKey key, ParsingContext context) {
    
    EditorState& state = context.state;
    Position cursor = state.getCursor().getPosition();

    switch (key) {
    case SpecialKey::ESCAPE: {
        return {ModeType::TOOL_MODE, {}};
    }

    case SpecialKey::BACKSPACE: {
        Position delete_position = cursor;
        if (delete_position.column > 0) { // move left if possible
            delete_position.column--;
        }
        else if (delete_position.row > 0) { // otherwise move to end of prev if possible
            delete_position.row--;
            delete_position.column = state.getParagraph(delete_position.row).length();
        } 
        else {
            return {std::nullopt, {}};
        }

        return {ModeType::TYPING_MODE, std::make_shared<DeleteAction>(delete_position, delete_position, cursor)};
    }

    case SpecialKey::ENTER: {
        return trySmartListInsertion(context);
    }

    case SpecialKey::ARROW_LEFT: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::LEFT)
        }};
    }

    case SpecialKey::ARROW_DOWN: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::DOWN)
        }};
    }

    case SpecialKey::ARROW_UP: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::UP)
        }};
    }

    case SpecialKey::ARROW_RIGHT: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::RIGHT)
        }};
    }

    case SpecialKey::TAB: {
        return {ModeType::TYPING_MODE, {
            make_shared<IndentAction>(cursor.row, context.settings.getTabWidth())
        }};
    }

    case SpecialKey::SHIFT_TAB: {
        return {ModeType::TYPING_MODE, {
            make_shared<UnindentAction>(context.state.getCursor().getRow(), context.settings.getTabWidth())
        }};
    }

    default: {
        return {ModeType::TYPING_MODE, {}};
    }

    }
}

ParseResult TypingMode::trySmartListInsertion(ParsingContext context) {
    Position cursor = context.state.getCursor().getPosition();
    if (!shouldTrySmartList(context)) {
        return basicParagraphSplit(cursor);
    }

    std::optional<ParseResult> result = getStaticSmartlistInsertion(context);
    if (result.has_value()) {
        return *result;
    }
    
    result = getDynamicSmartListInsertion(context);
    if (result.has_value()) {
        return *result;
    }

    return basicParagraphSplit(cursor);
}

bool TypingMode::shouldTrySmartList(ParsingContext context) {
    const std::string& file_name = context.state.getFileName();
    return (
        file_name.ends_with(".md")
        || file_name.ends_with(".txt")
    );
}

ParseResult TypingMode::basicParagraphSplit(Position cursor) {
    return {ModeType::TYPING_MODE, {std::make_shared<ParagraphSplittingAction>(cursor)}
    }; 
}


std::optional<ParseResult> TypingMode::getStaticSmartlistInsertion(ParsingContext context) {
    Position cursor = context.state.getCursor().getPosition();
    const std::string& current_paragraph = context.state.getParagraph(cursor.row);
    std::string until_cursor = current_paragraph.substr(0, cursor.column);

    std::unordered_map<std::string, std::string> list_types = {
        {"- ", "- "},
        {"+ ", "+ "},
        {"- [ ] ", "- [ ] "},
        {"- [x] ", "- [ ] "},
        {">", ">"}
    };

    for (const auto& [prefix, continuation] : list_types) {
        //TODO maybe extract this 
        if (!StringHelpers::startsWithIgnoringWhitespace(until_cursor, prefix)) {
            continue;
        }
        
        // if the user presses enter at when the paragraph is only the prefix, the prefix is deleted
        // To escape from list when no more should be added
        if (prefix.length() == current_paragraph.length()) {
            Position delete_start = {cursor.row, 0};

            //never called for empty paragraph so underflow is not a problem
            Position delete_end = {cursor.row, static_cast<int>(current_paragraph.length()) - 1}; 
            return ParseResult{ModeType::TYPING_MODE, std::make_shared<DeleteAction>(delete_start, delete_end, cursor)};
        }
        
        size_t leading_spaces = StringHelpers::countLeadingSpaces(until_cursor);
        std::string new_paragraph_prefix = std::string(leading_spaces, ' ') + continuation;

        ParseResult result = {
            ModeType::TYPING_MODE,
            {std::make_shared<ParagraphSplittingAction>(context.state.getCursor().getPosition(), new_paragraph_prefix)}
        };

        return result; 
    }

    return std::nullopt;
}

std::optional<ParseResult> TypingMode::getDynamicSmartListInsertion(ParsingContext context) {
    Position cursor = context.state.getCursor().getPosition();
    const std::string& paragraph = context.state.getParagraph(cursor.row);

    if (canBeNumberList(paragraph)) {
        return getNumberListInsertion(paragraph, cursor);
    }

    return std::nullopt;
}

bool TypingMode::canBeNumberList(const std::string& paragraph) {
    std::regex number_list_regex("^ *[0-9]+[.)] .*");
    return std::regex_match(paragraph, number_list_regex);
}

ParseResult TypingMode::getNumberListInsertion(const std::string& paragraph, Position cursor) {
    int numbering_value = getNumberingValue(paragraph);
    char numbering_ender = getNumberingEnder(paragraph);

    if (StringHelpers::countLeadingSpaces(paragraph) + std::to_string(numbering_value).length() + 2 == paragraph.length()) {
        // if the user presses enter at when the paragraph is only the prefix, the prefix is deleted
        // To escape from list when no more should be added
        Position delete_start = {cursor.row, 0};

        //never called for empty paragraph so underflow is not a problem
        Position delete_end = {cursor.row, static_cast<int>(paragraph.length()) - 1}; 

        return ParseResult{ModeType::TYPING_MODE, std::make_shared<DeleteAction>(delete_start, delete_end, cursor)};
    }

    std::string continuation = std::string(StringHelpers::countLeadingSpaces(paragraph), ' ')
        + std::to_string(numbering_value + 1)
        + numbering_ender
        + " ";
    
    ParseResult result = {
        ModeType::TYPING_MODE,
        {std::make_shared<ParagraphSplittingAction>(cursor, continuation)}
    };

    return result;
}

int TypingMode::getNumberingValue(const std::string& paragraph) {
    std::string after_whitespace = paragraph.substr(paragraph.find_first_not_of(' '));
    std::string number_only = after_whitespace.substr(0, StringHelpers::findFirstNonDigit(after_whitespace));

    return std::stoi(number_only);
}

char TypingMode::getNumberingEnder(const std::string& paragraph) {
    size_t dot = paragraph.find_first_of('.');
    size_t parentheses = paragraph.find_first_of(')');

    if (dot == std::string::npos && parentheses == std::string::npos) {
        throw std::invalid_argument("canBeNumberList() matched an illegal paragraph!");
    }

    if (dot == std::string::npos) {
        return ')';
    }

    if (parentheses == std::string::npos) {
        return '.';
    }

    return (dot < parentheses? '.' : ')');
}

ParseResult TypingMode::parseInput(Input input, ParsingContext context) {

    if (input.mouse_position.has_value()) {
        return parseMouseMovement(*input.mouse_position, context);
    }

    if (input.special_key.has_value()) {
        return parseSpecialKey(*input.special_key, context);
    }

    if (input.standard_input.has_value()) {
        std::vector<std::string> content = {std::string(1, *input.standard_input)};
        return {ModeType::TYPING_MODE, 
            std::make_shared<InsertAction>(content, context.state.getCursor().getPosition()),
        };
    } 
    
    return {ModeType::TYPING_MODE, std::nullopt};
}