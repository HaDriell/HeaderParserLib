#include <gtest/gtest.h>
#include <string>

#include "HeaderParserLib/Tokenizer.h"


TEST(TokenizerTests, SingleLineComment)
{
    Token token;
    Tokenizer tokenizer;

    tokenizer.SetSource("// Single line Comment ");
    ASSERT_TRUE(tokenizer.GetComment(token));
    ASSERT_TRUE(token.Type == TokenType::Comment);
    ASSERT_TRUE(token.Value == " Single line Comment ");
}

TEST(TokenizerTests, MultiLineComment)
{
    Token token;
    Tokenizer tokenizer;

    tokenizer.SetSource("/* Multi\nLine\nComment */");
    ASSERT_TRUE(tokenizer.GetComment(token));
    ASSERT_TRUE(token.Type == TokenType::Comment);
    ASSERT_TRUE(token.Value == " Multi\nLine\nComment ");
}

// int main()
// {
//     std::string input;
//     LoadFile("Example.cpp.input", input);

//     Tokenizer tokenizer;
//     tokenizer.SetSource(input);

//     Token token;

//     std::cout << "\n\n### Token Stream Begins\n\n";

//     while (tokenizer.GetToken(token))
//     {
//         if (token.Type == TokenType::Comment)       { std::cout << "Comment(" << token.Value << "\n"; }
//         if (token.Type == TokenType::Identifier)    { std::cout << "Identifier(" << token.Value << "\n"; }
//         if (token.Type == TokenType::Symbol)        { std::cout << "Symbol(" << token.Value << "\n"; }
//     }

//     std::cout << "\n\n### Token Stream Ends\n\n";
// }