#include <gtest/gtest.h>

#include "HeaderParserLib/Tokenizer.h"


TEST(TokenizerTests, SingleLineComment)
{
    Token token;
    Tokenizer tokenizer;

    tokenizer.SetSource("// Single line Comment ");
    ASSERT_TRUE(tokenizer.GetComment(token));
    ASSERT_TRUE(token.Type == TokenType::Comment);
    ASSERT_TRUE(token.Value == " Single line Comment ");

    tokenizer.SetSource("    //Comment\nIdentifier");
    ASSERT_TRUE(tokenizer.GetComment(token));
    ASSERT_TRUE(token.Type == TokenType::Comment);
    ASSERT_TRUE(token.Value == "Comment");
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

TEST(TokenizerTests, StarSymbol)
{
    Token token;
    Tokenizer tokenizer;

    tokenizer.SetSource("*");
    ASSERT_TRUE(tokenizer.GetSymbol(token));
    ASSERT_TRUE(token.Type == TokenType::Symbol);
    ASSERT_TRUE(token.Value == "*");
}

TEST(TokenizerTests, AlphaIdentifier)
{
    Token token;
    Tokenizer tokenizer;

    tokenizer.SetSource("a0");
    ASSERT_TRUE(tokenizer.GetIdentifier(token));
    ASSERT_TRUE(token.Type == TokenType::Identifier);
    ASSERT_TRUE(token.Value == "a0");
}

TEST(TokenizerTests, UnderscoredIdentifier)
{
    Token token;
    Tokenizer tokenizer;

    tokenizer.SetSource("_0a");
    ASSERT_TRUE(tokenizer.GetIdentifier(token));
    ASSERT_TRUE(token.Type == TokenType::Identifier);
    ASSERT_TRUE(token.Value == "_0a");
}

TEST(TokenizerTests, NoIdentifier)
{
    Token token;
    Tokenizer tokenizer;

    tokenizer.SetSource("123");
    ASSERT_FALSE(tokenizer.GetIdentifier(token));
}