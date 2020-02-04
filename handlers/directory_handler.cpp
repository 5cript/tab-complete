#include "directory_handler.hpp"

using namespace std::chrono_literals;
using namespace std::string_literals;
namespace fs = std::filesystem;

namespace TabCompletion
{
//#####################################################################################################################
    DirectoryHandler::DirectoryHandler(std::string baseDir)
        : baseDir_{std::move(baseDir)}
        , lastCacheUpdate_{std::chrono::system_clock::now() - 1h}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    CompletionResult DirectoryHandler::tryComplete(std::vector <Token> const& tokens) override
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void DirectoryHandler::updateCache()
    {
        auto now = std::chrono::system_clock::now();
        if (now - lastCacheUpdate_ < 5s)
            return;

        if (!fs::exists(baseDir_))
            return;

        cache_.clear();
        cache_.reserve(100);
        for (const auto & entry : fs::directory_iterator(baseDir_))
        {
            cache_.push_back(entry.path());
        }
    }
//#####################################################################################################################
}
