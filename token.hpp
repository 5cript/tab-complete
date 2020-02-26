#pragma once

#include <string>

namespace TabCompletion
{
    enum TokenType
    {
        Indeterminate,
        IdentifierAndPunct,
        Pipe,
        Space,
        Special
    };

    class Token
    {
    public:
        Token(TokenType type = TokenType::Indeterminate, std::string tok = "");
        TokenType type() const;
        std::string token() const;
        std::string& tokenHandle();

        /**
         *  Returns whether or not the token starts with " but does not end with ".
         */
        bool isUnfinishedQuote() const;

        /**
         *  If this this a block in quotation marks, return true;
         */
        bool isFinishedQuote() const;

        /**
         *  Returns the token content with escapes removed and interpreted.
         *  @return A pair, where first = escaped result, second = error message. If second is empty, no error occured.
         */
        std::pair <std::string, std::string> getEscaped() const;

    private:
        std::string tok_;
        TokenType type_;
    };
}
