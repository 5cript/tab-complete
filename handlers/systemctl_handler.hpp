#pragma once

#include "directory_handler.hpp"

#include "../completion_handler.hpp"

#include <filesystem>
#include <memory>
#include <optional>

namespace TabCompletion
{
    class SystemctlHandler
    {
    public:
        SystemctlHandler(DirectoryHandler* dHandler, std::optional <std::string> const& modulePath = std::nullopt);

        CompletionResult tryComplete(std::vector <Token> const& tokens, bool forceAll = false);

    private:
        struct Constants;

        DirectoryHandler* dHandler_;
        std::string modulePath_;
    };
}
