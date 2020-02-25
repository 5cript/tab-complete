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

    private:
        std::string tok_;
        TokenType type_;
    };
}
