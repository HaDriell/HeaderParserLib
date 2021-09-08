#include "HeaderParserLib/Tokenizer.h"
#include <regex>
#include <map>


Tokenizer::Tokenizer(std::string& input)
    : m_Stream(input)
{
}

Tokenizer::~Tokenizer()
{
}

bool Tokenizer::NextToken(Token& token)
{
    //Detect early end of input
    if (IsEOF())
    {
        return false;
    }

    //Skip whitespaces
    SkipWhitespaces();

    //Save cursor
    size_t lastValidPosition = m_Stream.tellg();

    //Try tokenizing
    if (NextIntegerLiteral(token))  return true; else Reset(lastValidPosition);
    if (NextStringLiteral(token))   return true; else Reset(lastValidPosition);
    if (NextIdentifier(token))      return true; else Reset(lastValidPosition);

    //Unexpected Token !
    return false;
}

bool Tokenizer::IsEOF()
{
    return m_Stream.eof();
}

void Tokenizer::Reset(size_t position)
{
    m_Stream.clear();
    m_Stream.seekg(position);
}

void Tokenizer::SkipWhitespaces()
{
    while(m_Stream.good() && std::isspace(m_Stream.peek()))
    {
        m_Stream.get();
    }
}

bool Tokenizer::Expect(const std::string& word)
{
    size_t position = m_Stream.tellg();

    for (char c : word)
    {
        if (c != m_Stream.get())
        {
            Reset(position);
            return false;
        }
    }
    
    return true;
}

bool Tokenizer::NextIntegerLiteral(Token& token)
{
    int64_t value;
    if (m_Stream >> value)
    {
        token = IntegerLiteralToken{ value };
        return true;
    }
    return false;
}

bool Tokenizer::NextStringLiteral(Token& token)
{
    char delimiter;
    m_Stream.get(delimiter);

    //Not a String Literal
    if (delimiter != '\'' && delimiter != '"')
    {
        return false;
    }

    std::string value;

    while (m_Stream.good())
    {
        if (m_Stream.eof()) return false; // Unexpected EOF

        char next;
        m_Stream.get(next);
        
        if (next == '\n') return false; // Unexpected newline

        //End of String
        if (next == delimiter)
        {
            token = StringLiteralToken{ value };
            return true;
        }

        //Buffer string
        value += next;
    }

    return false;
}

bool Tokenizer::NextIdentifier(Token& token)
{
    char c;
    m_Stream.get(c);

    std::string value = "";

    value.push_back(c);

    //Not an Identifier
    if (!std::isalpha(c)) return false;

    //Buffer the identifier
    while (m_Stream.good() && std::isalnum( std::char_traits<char>::to_char_type(m_Stream.peek()) ))
    {
        m_Stream.get(c);
        value += c;
    }

    token = IdentifierToken{ value };

    return true;
}