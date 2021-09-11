#pragma once

#include <cstdint>
#include <sstream>
#include <string>

#include "HeaderParserLib/Token.h"

class Tokenizer
{
public:
    void SetSource(const std::string& source);

    void ClearError();
    bool HasError();
    const std::string& GetErrorMessage();

    
    //Utility functions
    size_t GetPosition();
    void Reset(size_t position);
    bool Read(std::string& value, size_t count);
    bool ReadUntilSequence(const std::string& sequence, std::string& value);
    bool NextSequence(const std::string& sequence);
    bool NextWord(const std::string& word);
    void SkipWhitespaces();
    bool NextToken(Token& token);

    //Token functions
    bool NextComment(CommentToken& token);
    bool NextBooleanLiteral(BooleanLiteralToken& token);
    bool NextIntegerLiteral(IntegerLiteralToken& token);
    bool NextStringLiteral(StringLiteralToken& token);
    bool NextIdentifier(IdentifierToken& token);
    bool NextSymbol(SymbolToken& token);

private:
    //Error formatter
    bool Error(const char* fmt, ...);

private:
    bool m_HasError = false;
    std::string m_ErrorMessage;
    std::istringstream m_Stream;
};