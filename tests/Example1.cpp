#include <iostream>
#include <string>
#include <sstream>

#include "HeaderParserLib/Tokenizer.h"

std::string input = R"(
    class Test
    42
    "String Baby"
    3 2 1
    ""
    Error
    00
)";

class TokenLogger : public TokenHandler
{
public:
    void Handle(const IdentifierToken& token)       { std::cout << "IdentifierToken(" << token.Value << ")\n"; }
    void Handle(const IntegerLiteralToken& token)   { std::cout << "IntegerLiteral(" << token.Value << ")\n"; }
    void Handle(const StringLiteralToken& token)    { std::cout << "StringLiteral(" << token.Value << ")\n"; }
};


int main()
{
    Tokenizer tokenizer(input);
    TokenLogger logger;

    Token token;
    while (tokenizer.NextToken(token))
    {
        logger.HandleToken(token);
    }
}