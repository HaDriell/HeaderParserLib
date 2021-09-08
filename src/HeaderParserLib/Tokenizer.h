#pragma once

#include <cstdint>
#include <sstream>
#include <string>

#include "HeaderParserLib/Token.h"

class Tokenizer
{
public:
    Tokenizer(std::string& input);
    ~Tokenizer();

    Tokenizer(const Tokenizer& other) = delete;
    Tokenizer(Tokenizer&& other) = delete;

    bool NextToken(Token& token);

    void ClearError();
    bool HasError();
    const std::string& GetErrorMessage();

private:
    //Error formatter
    bool Error(const char* fmt, ...);
    
    //Utility functions
    void Reset(size_t position);
    bool Read(std::string& value, size_t count);
    bool ReadUntilSequence(const std::string& sequence, std::string& value);
    bool NextSequence(const std::string& sequence);
    bool NextWord(const std::string& word);

    //Token functions
    bool NextComment(Token& token);
    bool NextBooleanLiteral(Token& token);
    bool NextIntegerLiteral(Token& token);
    bool NextStringLiteral(Token& token);
    bool NextIdentifier(Token& token);
    bool NextSymbol(Token& token);

private:
    bool m_HasError;
    std::string m_ErrorMessage;
    std::istringstream m_Stream;
};