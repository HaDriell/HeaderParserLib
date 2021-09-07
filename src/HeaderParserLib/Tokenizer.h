#pragma once

#include <istream>
#include <ostream>
#include <vector>

#include "HeaderParserLib/Token.h"

class Tokenizer
{
public:
    Tokenizer(std::istream& stream);
    virtual ~Tokenizer();

    Tokenizer(const Tokenizer& other) = delete;
    Tokenizer(Tokenizer&& other) = delete;

    void Reset();
    char GetChar();
    char PeekChar();
    void UngetChar();
    bool IsEOF();
    bool NextToken();
private:
    Token m_CurrentToken;
    std::istream& m_Stream;
};