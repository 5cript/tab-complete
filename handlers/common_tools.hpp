#pragma once

#include "../completion_result.hpp"
#include "../token.hpp"
#include "directory_handler.hpp"

#include <vector>
#include <string>

namespace TabCompletion
{
    CompletionResult tryCompleteBaseCommands
    (
        std::vector <Token> const& tokens,
        bool forceAll,
        DirectoryHandler& dHandler,
        std::vector <std::string> const& dashCommands,
        std::vector <std::string> const& dashDashCommands,
        std::vector <std::string> const& otherCommands
    );
}
