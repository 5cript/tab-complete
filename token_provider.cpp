#include "token_provider.hpp"

#include <cctype>
#include <iostream>

namespace TabCompletion
{
//#####################################################################################################################
    void TokenProvider::append(std::string_view view)
    {
        std::size_t i = 0;

        if (!tokens_.empty())
        {
            auto& lastToken = tokens_.back();

            // merger behaviour
            if (lastToken.type() == TokenType::IdentifierAndPunct)
            {
                for (; i != view.size(); ++i)
                {
                    if (isIdentifierOrPunct(view[i]))
                        lastToken.tokenHandle().push_back(view[i]);
                    else
                        break;
                }
            }
        }

        // new toks
        Token curToken;
        for (; i != view.size(); ++i)
        {
#define PUSH_TOKEN_AND_REWIND \
    { \
        tokens_.push_back(curToken); \
        curToken = Token{}; \
        --i; \
        continue; \
    }

            char c = view[i];
            if (curToken.type() == TokenType::Indeterminate)
            {
                if (isIdentifierOrPunct(c))
                    curToken = Token{TokenType::IdentifierAndPunct};
                else if (isSpace(c))
                    curToken = Token{TokenType::Space};
                else if (c == '|')
                    curToken = Token{TokenType::Pipe};
                else if (isSpecial(c))
                    curToken = Token{TokenType::Special};
                curToken.tokenHandle().push_back(c);
            }
            else if (curToken.type() == TokenType::IdentifierAndPunct)
            {
                if (isIdentifierOrPunct(c))
                    curToken.tokenHandle().push_back(c);
                else
                    PUSH_TOKEN_AND_REWIND
            }
            else if (curToken.type() == TokenType::Space)
            {
                if (isSpace(c))
                    curToken.tokenHandle().push_back(c);
                else
                    PUSH_TOKEN_AND_REWIND
            }
            else if (curToken.type() == TokenType::Special)
            {
                if (isSpecial(c))
                    curToken.tokenHandle().push_back(c);
                else
                    PUSH_TOKEN_AND_REWIND
            }
        }

        if (curToken.type() != TokenType::Indeterminate)
            tokens_.push_back(curToken);
    }
//---------------------------------------------------------------------------------------------------------------------
    void TokenProvider::backspace()
    {
        if (tokens_.empty())
            return;

        if (tokens_.back().tokenHandle().size() == 1)
        {
            tokens_.pop_back();
            return;
        }

        tokens_.back().tokenHandle().pop_back();
    }
//---------------------------------------------------------------------------------------------------------------------
    void TokenProvider::clear()
    {
        tokens_.clear();
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TokenProvider::isIdentifierOrPunct(int c) const noexcept
    {
        return
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            std::ispunct(c)
        ;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TokenProvider::isSpace(int c) const noexcept
    {
        return c == ' ' || c == '\t';
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TokenProvider::isSpecial(int c) const noexcept
    {
        return !isIdentifierOrPunct(c) && !isSpace(c);
    }
//---------------------------------------------------------------------------------------------------------------------
    void TokenProvider::dump()
    {
        for (auto const& tok : tokens_)
        {
            std::cout << static_cast <int> (tok.type()) << ": '" << tok.token() << "'\n";
        }
    }
//#####################################################################################################################
}
