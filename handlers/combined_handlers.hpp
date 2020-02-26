#pragma once

#include "directory_handler.hpp"

#include "../completion_handler.hpp"

#include <filesystem>
#include <memory>

namespace TabCompletion
{
    /**
     */
    class CombinedHandler
    {
    public:
        CombinedHandler(DirectoryHandler* dHandler, std::optional <std::string> const& modulePath = std::nullopt);

        CompletionResult tryComplete(std::vector <Token> const& tokens, char pathSplitter, bool forceAll = false);

    private:
        struct Constants;

        DirectoryHandler* dHandler_;
        std::string modulePath_;
    };
}
