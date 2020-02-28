#include "combined_handlers.hpp"
#include "directory_handler.hpp"
#include "git_handler.hpp"
#include "systemctl_handler.hpp"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <cstdio>
#include <thread>
#include <chrono>
#include <iostream>

extern char **environ;

namespace TabCompletion
{
//#####################################################################################################################
    struct CombinedHandler::Constants
    {
    };
//#####################################################################################################################
    CombinedHandler::CombinedHandler(DirectoryHandler* dHandler, std::optional <std::string> const& modulePath)
        : dHandler_{dHandler}
    {
        if (modulePath)
            modulePath_ = modulePath.value();
        else
            modulePath_ = "/etc/systemd/system/";
    }
//---------------------------------------------------------------------------------------------------------------------
    CompletionResult CombinedHandler::tryComplete(std::vector <Token> tokens, char pathSplitter, bool forceAll)
    {
        if (tokens.empty())
            return {};

        auto res = std::find_if(std::rbegin(tokens), std::rend(tokens), [](auto const& tok){
            return (tok.type() == TokenType::Pipe);
        });

        if (tokens.front().token() == "sudo" && tokens.size() >= 2)
        {
            tokens.erase(std::begin(tokens), std::begin(tokens) + 2);
            //tokens.erase(std::begin(tokens));
        }

        auto start = std::begin(tokens);
        if (res != std::rend(tokens))
        {
            auto distFromBack = std::distance(std::rbegin(tokens), res);
            start += tokens.size() - distFromBack;
        }
        if (start->token() == "git")
        {
            auto git = GitHandler{dHandler_};
            auto res = git.tryComplete(std::vector <Token> {start, std::end(tokens)});
            if (!res.suggestions.empty())
                return res;
        }
        if (start->token() == "systemctl")
        {
            auto git = SystemctlHandler{dHandler_};
            auto res = git.tryComplete(std::vector <Token> {start, std::end(tokens)});
            if (!res.suggestions.empty())
                return res;
        }
        if (start->token() == "ls")
        {
            return dHandler_->tryComplete(tokens, forceAll, ScanningFilter::onlyDirectories());
        }
        if (start->token() == "cat")
        {
            return dHandler_->tryComplete(tokens, forceAll, ScanningFilter::onlyFiles());
        }

        if (tokens.size() == 1)
        {
            std::string front = tokens.front().token();

            if (front.size() >= 2 && front.substr(0, 2) == "./")
                return dHandler_->tryComplete(tokens);
            else
            {
                std::vector <std::string> paths;
                for (int i = 0; environ[i]; ++i)
                {
                    auto entry = std::string{environ[i]};
                    std::string var;
                    if (entry.size() > 5 && (entry.substr(0, 5) == "PATH=" || entry.substr(0, 5) == "Path="))
                    {
                        var = entry.substr(5, entry.size() - 5);
                        boost::split(paths, var, boost::is_any_of(std::string{pathSplitter}));
                    }
                }

                CompletionResult result;
                for (auto const& i : paths)
                {
                    DirectoryHandler dh{i};
                    auto res = dh.tryComplete(tokens);
                    result.suggestions.insert(result.suggestions.end(), res.suggestions.begin(), res.suggestions.end());
                }
                return result;
            }
        }
        else
        {
            return dHandler_->tryComplete(tokens, forceAll);
        }

        return {};
    }
//#####################################################################################################################
}
