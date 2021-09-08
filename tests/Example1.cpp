#include <iostream>
#include <string>
#include <sstream>

#include "HeaderParserLib/Tokenizer.h"

std::string input = R"(
    #pragma once
    #include <something>
    #ifdef SOMETHING
    #include "HeaderLib/Tokenizer.h"
    #endif

    ICI /*
        multiline
        comment !
    */

    CLASS() // Single line comment ;)
    class Token {
    public:
        PROPERTY()
        uint32_t m_PropertyName = 32;
    };
)";

class TokenLogger : public TokenHandler
{
public:
    void Handle(const SymbolToken& token)           { std::cout << "Symbol( " << token.Value << " )\n"; }
    void Handle(const CommentToken& token)          { std::cout << "Comment( " << token.Value << " )\n"; }
    void Handle(const IdentifierToken& token)       { std::cout << "Identifier( " << token.Value << " )\n"; }
    void Handle(const IntegerLiteralToken& token)   { std::cout << "Integer( " << token.Value << " )\n"; }
    void Handle(const StringLiteralToken& token)    { std::cout << "String( " << token.Value << " )\n"; }
};


int main()
{
    Tokenizer tokenizer(input);
    TokenLogger logger;

    Token token;
    std::cout << "\n\n### Token Stream Begins\n\n";
    while (tokenizer.NextToken(token))
    {
        logger.HandleToken(token);
    }
    std::cout << "\n\n### Token Stream Ends\n\n";
}