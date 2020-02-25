#pragma once

#include <vector>
#include <string>

namespace TabCompletion
{
    struct CompletionResult
    {
        std::vector <std::string> suggestions;
        bool inappropriateHandler;

        CompletionResult()
            : suggestions{}
            , inappropriateHandler{false}
        {
        }

        CompletionResult(std::vector <std::string> suggs)
            : suggestions{std::move(suggs)}
            , inappropriateHandler{false}
        {
        }

        static CompletionResult makeInappropriateResult()
        {
            CompletionResult res;
            res.inappropriateHandler = true;
            return res;
        }
    };
}
