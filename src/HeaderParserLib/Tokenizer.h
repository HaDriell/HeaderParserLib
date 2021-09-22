#pragma once

#include <cstdint>
#include <sstream>
#include <string>

#include "HeaderParserLib/Token.h"

class Tokenizer
{
public:
    //////////////////////////////////////////////////////////////////////
    // Source Stream Manipulator Functions

    /// Create a new Source Stream using the provided source
    void SetSource(const std::string& source);

    /// Gets the current position in the Source Stream
    size_t GetPosition();

    /// Sets the current position in the Source Stream
    void SetPosition(size_t position, bool clearErrors = true);

    /// Gets the next character in the Source Stream
    /// Fails if Source Stream is bad
    bool Get(char& character);

    /// Gets the next <count> characters in the Source Stream and stores them in <value>
    /// Fails if Source Stream is bad
    bool GetString(std::string& value, size_t count);

    /// Gets the next characters from the Source Stream until the <delimiter> is output. <delimiter> is not added to the value
    /// Fails if Source Stream is bad
    bool GetString(std::string& value, const std::string& delimiter);

    /// Tests that the next characters in the Source Stream matches <expected> 
    /// Fails if the string doesn't match
    /// Fails if Source Stream is bad
    bool ExpectString(const std::string& expected);

    /// Gets the next 'word'. A word is a string only composed of non-whitespace characters
    /// Fails if the word is empty
    /// Fails if Source Stream is bad
    bool GetWord(std::string& value);

    /// Tests that the next characters in the Source Stream matches the word <expected>
    /// Fails if the word doesn't match
    /// Fails if Source Stream is bad
    bool ExpectWord(const std::string& expected);

    /// Consumes characters in the Source Stream until it peeks a non-Whitespace character
    void SkipWhitespaces();

    //////////////////////////////////////////////////////////////////////
    // Tokenization Functions

    /// Tries to parse a Comment (single or multi-line) from the Source Stream
    /// Rollbacks the Source Stream on fail
    bool GetComment(Token& token);

    /// Tries to parse an identifier from the Source Stream
    /// Rollbacks the Source Stream on fail
    bool GetIdentifier(Token& token);

    /// Tries to parse a Symbol from the Source Stream
    /// Rollbacks the Source Stream on fail
    bool GetSymbol(Token& token);

    /// Tries to Get every type of token
    /// Fails if no token type was successfully parsed
    bool GetToken(Token& token);

    /// Tries to parse an Identifier with the matching <expected> value
    /// Rollbacks the Source Stream on fail
    bool ExpectIdentifier(const std::string& expected);
    
    /// Tries to parse a Symbol with the matching <expected> value
    /// Rollbacks the Source Stream on fail
    bool ExpectSymbol(const std::string& expected);

private:
    std::istringstream m_Stream;
};