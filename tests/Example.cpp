#include <iostream>
#include <string>

#include "HeaderParserLib/Tokenizer.h"
#include "ExampleUtils.h"


int main()
{
    std::string input;
    LoadFile("Example.cpp.input", input);

    Tokenizer tokenizer;
    tokenizer.SetSource(input);

    Token token;

    std::cout << "\n\n### Token Stream Begins\n\n";

    while (tokenizer.GetToken(token))
    {
        if (token.Type == TokenType::Comment)       { std::cout << "Comment(" << token.Value << "\n"; }
        if (token.Type == TokenType::Identifier)    { std::cout << "Identifier(" << token.Value << "\n"; }
        if (token.Type == TokenType::Symbol)        { std::cout << "Symbol(" << token.Value << "\n"; }
    }

    std::cout << "\n\n### Token Stream Ends\n\n";
}