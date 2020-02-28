#pragma once

#include "../completion_handler.hpp"

#include <string>
#include <chrono>
#include <optional>
#include <filesystem>

namespace TabCompletion
{
    struct ScanningFilter
    {
        bool directories = true;
        bool files = true;
        bool symlinks = true;
        std::optional <std::string> extension = std::nullopt;

        static ScanningFilter onlyDirectories()
        {
            return {true, false, true, std::nullopt};
        }

        static ScanningFilter onlyFiles()
        {
            return {false, true, true, std::nullopt};
        }

        template <typename DirectoryEntry>
        bool satisfiesFilter(DirectoryEntry const& entry) const
        {
            if (!directories && entry.is_directory())
                return false;

            if (!files && entry.is_regular_file())
                return false;

            if (!symlinks && entry.is_symlink())
                return false;

            if (extension)
                return entry.path().extension() == extension.value();

            return true;
        }
    };

    class DirectoryHandler
    {
    public:
        constexpr static auto basicCacheMax = 200;

        DirectoryHandler(std::filesystem::path baseDir, bool recursive = false);

        bool setDirectoryMode(bool dirmode);

        CompletionResult tryComplete(std::vector <Token> const& tokens, bool forceAll = false, ScanningFilter const& filter = {});

        /**
         *  There may have been some exceptions along the way. These get ignored, so that no suggestions get returned.
         *  But in some cases you want to know why there are no suggestions even if seemingly valid input should have given
         *  you some. This variable is reset with every tryComplete call.
         */
        std::string lastError() const;

    private:
        /// update cache if older than X seconds
        void updateCache(bool forceAll, ScanningFilter const& filter);

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
