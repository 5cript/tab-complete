#include "token.hpp"

#include "utf-8.hpp"

using namespace std::string_literals;

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
    bool Token::isFinishedQuote() const
    {
        if (tok_.empty())
            return false;

        if (tok_.front() == '"')
            return !isUnfinishedQuote();

        return false;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::pair <std::string, std::string> Token::getEscaped() const
    {
        std::string escaped = tok_;

        if (isFinishedQuote())
            escaped = escaped.substr(1, escaped.size() - 2);

        std::string filtered;

#define GUARD_AHEAD_ACCESS(X, action) \
        if (i + X >= escaped.size()) action

#define EXTEND_AND_CONTINUE(X) \
    { \
        filtered += X; \
        ++i; \
        continue; \
    }

#define RETURN_ERROR(Message) \
    return {filtered, Message}

        for (std::size_t i = 0; i != escaped.size(); ++i)
        {
            if (escaped[i] == '\\')
            {
                GUARD_AHEAD_ACCESS(1, continue);

                if (escaped[i + 1] == 'n')
                    EXTEND_AND_CONTINUE("\n")
                if (escaped[i + 1] == 'r')
                    EXTEND_AND_CONTINUE("\r")
                if (escaped[i + 1] == 't')
                    EXTEND_AND_CONTINUE("\t")
                if (escaped[i + 1] == 'a')
                    EXTEND_AND_CONTINUE("\a")
                if (escaped[i + 1] == 'f')
                    EXTEND_AND_CONTINUE("\f")
                if (escaped[i + 1] == 'v')
                    EXTEND_AND_CONTINUE("\v")
                if (escaped[i + 1] == '\\')
                    EXTEND_AND_CONTINUE("\\")
                if (escaped[i + 1] == '"')
                    EXTEND_AND_CONTINUE("\"")
                if (escaped[i + 1] == '"')
                    EXTEND_AND_CONTINUE("\"")
                if (escaped[i + 1] == 'e')
                    EXTEND_AND_CONTINUE("\x1b")
                if (escaped[i + 1] == 'u')
                {
                    // hexadecimal guaranteed 4 chars.
                    GUARD_AHEAD_ACCESS(4, RETURN_ERROR("invalid escape sequence for unicode hexadecimals u"));

                    auto page = std::stoul(escaped.substr(i + 2, 4), nullptr, 16);
                    filtered += codepageToUtf8(page);
                    i += 5;
                    continue;
                }
                if (escaped[i + 1] == 'U')
                {
                    GUARD_AHEAD_ACCESS(8, RETURN_ERROR("invalid escape sequence for unicode hexadecimals U"));

                    auto page = std::stoul(escaped.substr(i + 2, 8), nullptr, 16);
                    i += 9;
                    filtered += codepageToUtf8(page);
                    continue;
                }
                if (escaped[i + 1] >= '0' && escaped[i + 1] <= '9')
                {
                    // octal sequence
                    int width = 1;
                    if (i + 2 >= escaped.size() && escaped[i + 2] >= '0' && escaped[i + 2] <= '9')
                        width = 2;

                    if (i + 3 >= escaped.size() && escaped[i + 3] >= '0' && escaped[i + 3] <= '9')
                        width = 3;

                    auto oct = std::stoul(escaped.substr(i + 1, width), nullptr, 8);
                    filtered += codepageToUtf8(oct);
                    i += width + 1;
                    continue;
                }
                if (escaped[i + 1] == 'x')
                {
                    auto j = i + 2;
                    for (; j < escaped.size(); ++j)
                    {
                        auto c = escaped[j];
                        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
                            break;
                    }

                    if (j == i + 2)
                        RETURN_ERROR("hexadecimal escape sequence is invalid");

                    filtered += codepageToUtf8(std::stoul(escaped.substr(i + 2, j - i - 2), nullptr, 16));
                    i += (j - i) - 1;
                    continue;
                }

                filtered.push_back(escaped[i]);
            }
            else
                filtered.push_back(escaped[i]);

        }

        return {filtered, {}};
    }
//---------------------------------------------------------------------------------------------------------------------
    bool Token::isUnfinishedQuote() const
    {
        if (tok_.empty())
            return false;

        if (tok_.front() == '"')
        {
            if (tok_.back() == '"')
            {
                int slashCount = 0;
                for (auto i = tok_.rbegin() + 1; i > tok_.rend(); ++i)
                {
                    if (*i == '\\')
                        slashCount++;
                    else
                        break;
                }
                return slashCount % 2 != 0;
            }
        }

        return false;
    }
//---------------------------------------------------------------------------------------------------------------------
    TokenType Token::type() const
    {
        return type_;
    }
//#####################################################################################################################
}
