#pragma once

#include "completion_result.hpp"
#include "token.hpp"

namespace TabCompletion
{
    class GenericCompletionHandler
    {
    public:
        /// Try to find a completion for the given query.
        virtual CompletionResult tryComplete(std::vector <Token> const& tokens) const = 0;

        virtual ~GenericCompletionHandler() = default;
    private:
    };
}
