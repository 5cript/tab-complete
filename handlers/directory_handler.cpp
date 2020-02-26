#include "directory_handler.hpp"

#include <algorithm>
#include <type_traits>

using namespace std::chrono_literals;
using namespace std::string_literals;
namespace fs = std::filesystem;

namespace TabCompletion
{
//#####################################################################################################################
    DirectoryHandler::DirectoryHandler(std::filesystem::path baseDir, bool recursive)
        : baseDir_{std::move(baseDir)}
        , cachedPath_{baseDir_}
        , lastCachedPath_{}
        , lastCacheUpdate_{std::chrono::system_clock::now() - 1h}
        , localCache_{}
        , lastError_{}
        , recursive_{recursive}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    CompletionResult DirectoryHandler::tryComplete(std::vector <Token> const& tokens, bool forceAll)
    {
        lastError_ = "";

        Token tok;
        if (!tokens.empty())
            tok = tokens.back();

        std::string basePath = "";

        if (!tokens.empty() && tok.type() == TokenType::IdentifierAndPunct)
        {
            auto baseTry = tok.getEscaped();
            if (!baseTry.second.empty())
                return CompletionResult{};
            basePath = baseTry.first;
        }
        else if (!tokens.empty())
        {
            basePath = "";
        }

        // only use forward slash in paths
        std::replace(std::begin(basePath), std::end(basePath), '\\', '/');

        bool searchLocal = false;

        std::string fileNamePart = "";

        auto slashCount = std::count(std::begin(basePath), std::end(basePath), '/');
        if (slashCount == 0)
        {
            fileNamePart = basePath;
            searchLocal = true;
        }
        else if (slashCount == 1 && basePath.size() >= 2 && basePath.substr(0, 2) == "./")
        {
            fileNamePart = basePath.substr(2, basePath.size() - 2);
            searchLocal = true;
        }
        else
        {
            // simple cases are out the window:
            try
            {
                fs::path p{basePath};

                if (p.parent_path() == baseDir_ || p == baseDir_)
                    searchLocal = true;
                else
                {
                    try
                    {
                        cachedPath_ = p.parent_path();
                        updateCache(forceAll);

                        fileNamePart = p.filename().string();
                    }
                    catch (...)
                    {
                        return {};
                    }
                }
            }
            catch (...)
            {
                return {};
            }
        }

        if (searchLocal)
        {
            cachedPath_ = baseDir_;
            updateCache(forceAll);
        }

        CompletionResult res{};
        for (auto const& p : localCache_)
        {
            auto cachedName = p.filename().string();

            if (cachedName.size() < fileNamePart.size())
                continue;
            if (cachedName.substr(0, fileNamePart.size()) == fileNamePart)
                res.suggestions.push_back(p.filename().string());
        }

        /*
        if (res.suggestions.size() == 1)
        {
            auto& f = res.suggestions.front();
            f = f.substr(fileNamePart.size(), f.size() - fileNamePart.size());
        }
        */

        return res;
    }
//---------------------------------------------------------------------------------------------------------------------
    void DirectoryHandler::updateCache(bool forceAll)
    {
        try
        {
            if (lastCachedPath_ == cachedPath_)
            {
                auto now = std::chrono::system_clock::now();
                if (now - lastCacheUpdate_ < 3s)
                    return;
            }

            if (!fs::exists(cachedPath_))
                return;

            localCache_.clear();
            auto fillCache = [this, forceAll](auto typeDummy)
            {
                using iterator_t = std::decay_t <decltype(typeDummy)>;

                if (!forceAll)
                {
                    localCache_.reserve(25);
                    int c = basicCacheMax;
                    for (const auto & entry : iterator_t{cachedPath_})
                    {
                        localCache_.push_back(entry.path());
                        --c;
                        if (c <= 0)
                            break;
                    }
                }
                else
                {
                    localCache_.reserve(1000);
                    for (const auto & entry : iterator_t{cachedPath_})
                        localCache_.push_back(entry.path().filename());
                }
            };

            if (recursive_)
                fillCache(fs::recursive_directory_iterator{});
            else
                fillCache(fs::directory_iterator{});
        }
        catch(std::exception const& exc)
        {
            lastError_ = exc.what();
        }
        catch(...)
        {
            lastError_ = "unknown exception in directory traversal";
        }

        lastCachedPath_ = cachedPath_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string DirectoryHandler::lastError() const
    {
        return lastError_;
    }
//#####################################################################################################################
}
