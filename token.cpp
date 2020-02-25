#include "token.hpp"

namespace TabCompletion
{
//#####################################################################################################################
    Token::Token(TokenType type, std::string tok)
        : tok_{std::move(tok)}
        , type_{type}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Token::token() const
    {
        return tok_;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string& Token::tokenHandle()
    {
        return tok_;
    }
//---------------------------------------------------------------------------------------------------------------------
    TokenType Token::type() const
    {
        return type_;
    }
//#####################################################################################################################
}
