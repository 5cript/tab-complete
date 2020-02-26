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
        constexpr static auto basicCacheMax = 200;

        DirectoryHandler(std::filesystem::path baseDir, bool recursive = false);

        CompletionResult tryComplete(std::vector <Token> const& tokens, bool forceAll = false);

        /**
         *  There may have been some exceptions along the way. These get ignored, so that no suggestions get returned.
         *  But in some cases you want to know why there are no suggestions even if seemingly valid input should have given
         *  you some. This variable is reset with every tryComplete call.
         */
        std::string lastError() const;

    private:
        /// update cache if older than X seconds
        void updateCache(bool forceAll);

    private:
        std::filesystem::path baseDir_;
        std::filesystem::path cachedPath_;
        std::filesystem::path lastCachedPath_;
        std::chrono::system_clock::time_point lastCacheUpdate_;
        std::vector <std::filesystem::path> localCache_;
        std::string lastError_;
        bool recursive_;
    };
}
