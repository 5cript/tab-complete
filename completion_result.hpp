#pragma once

namespace TabCompletion
{
    struct CompletionResult
    {
        bool hasOptions;
        std::vector <std::string> suggestions;
    };
}
