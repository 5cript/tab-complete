#pragma once

#include "directory_handler.hpp"

#include "../completion_handler.hpp"

#include <filesystem>
#include <memory>

namespace TabCompletion
{
    class GitHandler
    {
    public:
        /**
         *  Passing in directory handler to make use of caching.
         */
        GitHandler(DirectoryHandler* dHandler);

        CompletionResult tryComplete(std::vector <Token> const& tokens, bool forceAll = false);

    private:
        struct Constants;

        DirectoryHandler* dHandler_;
    };

}
