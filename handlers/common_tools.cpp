#include "common_tools.hpp"

using namespace std::string_literals;

namespace TabCompletion
{
//#####################################################################################################################
    CompletionResult tryCompleteBaseCommands
    (
        std::vector <Token> const& tokens,
        bool forceAll,
        DirectoryHandler& dHandler,
        std::vector <std::string> const& dashCommands,
        std::vector <std::string> const& dashDashCommands,
        std::vector <std::string> const& otherCommands
    )
    {
        if (tokens.size() == 2 && tokens.back().token() == " ")
        {
            CompletionResult res;
            for (auto i = std::begin(otherCommands); i != std::end(otherCommands); ++i)
                res.suggestions.push_back(*i);
            return res;
        }

        // "COMMAND -"
        if (tokens.size() == 3 && tokens.back().token() == "-")
        {
            CompletionResult res;
            for (auto i = std::begin(dashCommands); i != std::end(dashCommands); ++i)
                res.suggestions.push_back("-"s + *i);
            return res;
        }

        // "COMMAND --"
        if (tokens.size() == 3 && tokens.back().token() == "--")
        {
            CompletionResult res;
            for (auto i = std::begin(dashDashCommands); i != std::end(dashDashCommands); ++i)
                res.suggestions.push_back("--"s + *i);
            return res;
        }

        // "COMMAND ?"
        if (tokens.size() == 3 && tokens.back().type() == TokenType::IdentifierAndPunct)
        {
            CompletionResult res;
            auto checkAgainst = tokens.back().token();
            for (auto i = std::begin(otherCommands); i != std::end(otherCommands); ++i)
            {
                if (i->size() >= checkAgainst.size() && i->substr(0, checkAgainst.size()) == checkAgainst)
                    res.suggestions.push_back(*i);
            }
            if (checkAgainst.size() > 2 && checkAgainst.substr(0, 2) == "--")
            {
                checkAgainst = checkAgainst.substr(2, checkAgainst.size() - 2);
                for (auto i = std::begin(dashDashCommands); i != std::end(dashDashCommands); ++i)
                {
                    if (i->size() >= checkAgainst.size() && i->substr(0, checkAgainst.size()) == checkAgainst)
                        res.suggestions.push_back("--"s + *i);
                }
            }
            return res;
        }

        // "COMMAND c ?"
        if (tokens.size() > 3)
        {
            std::vector <Token> tokenCut;
            if (tokens.back().type() == TokenType::IdentifierAndPunct)
                tokenCut.push_back(tokens.back());

            return dHandler.tryComplete(tokenCut, forceAll);
        }

        return {};
    }
//---------------------------------------------------------------------------------------------------------------------
//#####################################################################################################################
}
