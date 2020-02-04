#pragma once

#include "../completion_handler.hpp"

#include <string>
#include <chrono>
#include <filesystem>

namespace TabCompletion
{
    class DirectoryHandler : public GenericCompletionHandler
    {
    public:
        DirectoryHandler(std::path baseDir);

        CompletionResult tryComplete(std::vector <Token> const& tokens) const override;

    private:
        /// update cache if older than X seconds
        void updateCache();

    private:
        std::path baseDir_;
        std::chrono::system_clock::time_point lastCacheUpdate_;
        std::vector <std::path> cache_;
    };
}
