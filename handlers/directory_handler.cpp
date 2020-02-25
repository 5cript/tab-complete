#include "directory_handler.hpp"

using namespace std::chrono_literals;
using namespace std::string_literals;
namespace fs = std::filesystem;

namespace TabCompletion
{
//#####################################################################################################################
    DirectoryHandler::DirectoryHandler(std::filesystem::path baseDir)
        : baseDir_{std::move(baseDir)}
        , lastCacheUpdate_{std::chrono::system_clock::now() - 1h}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    CompletionResult DirectoryHandler::tryComplete(std::vector <Token> const& tokens, bool forceAll)
    {
        updateCache(forceAll);

        if (cache_.empty())
            return {{}};

        Token tok;
        if (!tokens.empty())
            tok = tokens.back();

        if (tok.type() != TokenType::IdentifierAndPunct)
        {
            CompletionResult res{{}};
            for (auto const& p : cache_)
                res.suggestions.push_back(p.string());
            return res;
        }

        CompletionResult res{{}};
        for (auto const& p : cache_)
        {
            auto view = std::string_view{p.string()};
            if (view.size() < tok.tokenHandle().size())
                continue;
            if (view.substr(0, tok.tokenHandle().size()) == tok.tokenHandle())
                res.suggestions.push_back(p.string());
        }
        return res;
    }
//---------------------------------------------------------------------------------------------------------------------
    void DirectoryHandler::updateCache(bool forceAll)
    {
        auto now = std::chrono::system_clock::now();
        if (now - lastCacheUpdate_ < 5s)
            return;

        if (!fs::exists(baseDir_))
            return;

        cache_.clear();
        if (!forceAll)
        {
            cache_.reserve(25);
            int c = basicCacheMax;
            for (const auto & entry : fs::directory_iterator(baseDir_))
            {
                cache_.push_back(entry.path());
                --c;
                if (c <= 0)
                    break;
            }
        }
        else
        {
            cache_.reserve(1000);
            for (const auto & entry : fs::directory_iterator(baseDir_))
                cache_.push_back(entry.path().filename());
        }
    }
//#####################################################################################################################
}
