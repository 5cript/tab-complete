#pragma once

#include "../completion_handler.hpp"

#include <filesystem>
#include <memory>

namespace TabCompletion
{
    class GitHandler
    {
    public:
        GitHandler(std::filesystem::path baseDir);

        CompletionResult tryComplete(std::vector <Token> const& tokens, bool forceAll = false);

    private:
        struct Constants;

        std::filesystem::path baseDir_;
    };

}
