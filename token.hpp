#pragma once

namespace TabCompletion
{
    enum TokenType
    {
        Identifier,
        Operator,
        Pipe,
        Space
    };

    class Token
    {
    public:
        Token(std::string tok, TokenType type);
        TokenType type() const;
        std::string token() const;
        std::string const& tokenHandle() const;

    private:
        std::string tok_;
        TokenType type_;
    };
}
