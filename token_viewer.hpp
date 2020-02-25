#pragma once

#include "token.hpp"

#include <vector>

namespace TabCompletion
{
    /**
     *  Can be used to inspect a set of tokens.
     */
    class TokenViewer
    {
    public:
        TokenViewer(std::vector <Token> const* tokens);

        bool endsWithSpace() const;
        bool endsWithPipe() const;
        bool empty() const;
        bool endsInCharacters() const;

        /**
         *  Gets the last identifier, that is not the last token and only if nothing else but space follows the last.
         *
         *  Examples:
         *      - "git a" -> tokens: {"git", " ", "a"} -> returns "git"
         *      - "git " -> tokens: {"git", " "} -> returns "git"
         *      - " a" -> returns nullptr
         *      - "a |" -> returns nullptr
         *      - "a | grep" returns nullptr. <- !important case
         *
         *  @return nullptr or last identifier that is not the exact last.
         */
        Token const* secondToLastIdent() const;

    private:
        Token const& end() const;

    private:
        std::vector <Token> const* tokens_;
    };
}
