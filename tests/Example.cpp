#include <iostream>
#include <string>

#include "HeaderParserLib/Tokenizer.h"
#include "ExampleUtils.h"

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
    std::string input;
    LoadFile("Example.cpp.input", input);

    Tokenizer tokenizer(input);
    TokenLogger logger;

    Token token;

    std::cout << "\n\n### Token Stream Begins\n\n";

    while (tokenizer.NextToken(token))
    {
        logger.HandleToken(token);
    }

    if (tokenizer.HasError())
    {
        std::cout << tokenizer.GetErrorMessage();
    }

    std::cout << "\n\n### Token Stream Ends\n\n";
}