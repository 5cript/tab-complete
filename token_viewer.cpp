#include "token_viewer.hpp"

namespace TabCompletion
{
//#####################################################################################################################
    TokenViewer::TokenViewer(std::vector <Token> const* tokens)
        : tokens_{tokens}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    Token const& TokenViewer::end() const
    {
        return *tokens_->rbegin();
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TokenViewer::endsWithSpace() const
    {
        if (tokens_->empty())
            return false;

        return end().type() == TokenType::Space;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TokenViewer::endsWithPipe() const
    {
        if (tokens_->empty())
            return false;

        return end().type() == TokenType::Pipe;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TokenViewer::empty() const
    {
        return tokens_->empty();
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TokenViewer::endsInCharacters() const
    {
        if (tokens_->empty())
            return false;

        return end().type() == TokenType::IdentifierAndPunct;
    }
//---------------------------------------------------------------------------------------------------------------------
    Token const* TokenViewer::secondToLastIdent() const
    {
        if (tokens_->empty())
            return nullptr;

        const auto tokSize = tokens_->size();

        if (end().type() == TokenType::Space && tokSize >= 2)
            return &tokens_->operator[](tokSize - 2);

        if (end().type() == TokenType::IdentifierAndPunct && tokSize >= 3)
        {
            if (tokens_->operator[](tokSize - 2).type() != TokenType::Space)
                return nullptr;
            else
                return &tokens_->operator[](tokSize - 3);
        }

        return nullptr;
    }
//#####################################################################################################################
}
