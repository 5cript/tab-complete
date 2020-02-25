#pragma once

#include "../completion_handler.hpp"

#include <string>
#include <chrono>
#include <filesystem>

namespace TabCompletion
{
    class DirectoryHandler
    {
    public:
        constexpr static auto basicCacheMax = 1000;

        DirectoryHandler(std::filesystem::path baseDir);

        CompletionResult tryComplete(std::vector <Token> const& tokens, bool forceAll = false);

    private:
        /// update cache if older than X seconds
        void updateCache(bool forceAll);

    private:
        std::filesystem::path baseDir_;
        std::chrono::system_clock::time_point lastCacheUpdate_;
        std::vector <std::filesystem::path> cache_;
    };
}
