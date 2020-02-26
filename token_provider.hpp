#pragma once

#include "token.hpp"

#include <vector>
#include <string_view>

namespace TabCompletion
{
    class TokenProvider
    {
    public:
        /**
         *  Appends characters to the token storage.
         *  the appended data will be tokenized and merged with the last token if needed.
         *  eg:
         *      Tokens are: 'this' SPACE 'is'
         *      data to append is: 'dat'
         *      Result: 'this' SPACE' 'isdat'.
         */
        void append(std::string_view data);

        /**
         *  Remove from tokens as if backspace was pressed.
         */
        void backspace();

        /**
         *  Removes everything from this store.
         */
        void clear();

        void dump();

        std::vector <Token> const& tokens() const;

    private:
        bool isIdentifierOrPunct(int c) const noexcept;
        bool isSpace(int c) const noexcept;
        bool isSpecial(int c) const noexcept;

    private:
        std::vector <Token> tokens_;
    };
}
