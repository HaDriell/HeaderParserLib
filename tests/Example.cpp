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

    while (tokenizer.NextToken(token))
    {
        if (std::holds_alternative<SymbolToken>(token))         { std::cout << "Symbol( " << std::get<SymbolToken>(token).Value << " )\n"; }
        if (std::holds_alternative<CommentToken>(token))        { std::cout << "Comment( " << std::get<CommentToken>(token).Value << " )\n"; }
        if (std::holds_alternative<IdentifierToken>(token))     { std::cout << "Identifier( " << std::get<IdentifierToken>(token).Value << " )\n"; }
        if (std::holds_alternative<IntegerLiteralToken>(token)) { std::cout << "Integer( " << std::get<IntegerLiteralToken>(token).Value << " )\n"; }
        if (std::holds_alternative<StringLiteralToken>(token))  { std::cout << "String( " << std::get<StringLiteralToken>(token).Value << " )\n"; }
    }

    if (tokenizer.HasError())
    {
        std::cout << tokenizer.GetErrorMessage();
    }

    std::cout << "\n\n### Token Stream Ends\n\n";
}