#pragma once

#include <cstdint>
#include <sstream>
#include <string>

#include "HeaderParserLib/Token.h"

class Tokenizer final
{
public:
    Tokenizer(std::string& input);
    ~Tokenizer();

    Tokenizer(const Tokenizer& other) = delete;
    Tokenizer(Tokenizer&& other) = delete;

    bool NextToken(Token& token);
    bool IsEOF();

private:
    void Reset(size_t position);
    void SkipWhitespaces();
    bool Expect(const std::string& word);

    bool NextIntegerLiteral(Token& token);
    bool NextStringLiteral(Token& token);
    bool NextIdentifier(Token& token);
private:
    std::istringstream m_Stream;
};