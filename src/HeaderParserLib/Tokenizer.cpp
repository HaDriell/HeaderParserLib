#include "HeaderParserLib/Tokenizer.h"
#include <regex>
#include <map>


Tokenizer::Tokenizer(std::string& input)
    : m_Stream(input)
{
    m_Stream >> std::noskipws;
}

Tokenizer::~Tokenizer()
{
}

bool Tokenizer::NextToken(Token& token)
{
    //skip whitespaces
    while (std::isspace(m_Stream.peek()))
    {
        m_Stream.get();
    }

    //Detect early end of input
    if (IsEOF())
    {
        return false;
    }

    //Save cursor
    size_t position = m_Stream.tellg();

    // Check for Comments
    if (NextComment(token))             return true; else Reset(position);
    // Check for Constants & Literals
    if (NextIntegerLiteral(token))      return true; else Reset(position);
    if (NextStringLiteral(token))       return true; else Reset(position);
    if (NextBooleanLiteral(token))      return true; else Reset(position);
    // Check for identifiers
    if (NextIdentifier(token))          return true; else Reset(position);
    // Check for symbols
    if (NextSymbol(token))              return true; else Reset(position);

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

bool Tokenizer::Read(std::string& value, size_t count)
{
    size_t position = m_Stream.tellg();

    value.resize(count);
    for (char& c : value)
    {
        m_Stream >> c;
    }

    if (m_Stream.bad())
    {
        Reset(position);
        return false;
    }

    return true;
}

bool Tokenizer::ReadUntilSequence(const std::string& sequence, std::string& value)
{
    size_t position = m_Stream.tellg();
    
    std::string buffer;
    char next;

    while (!NextSequence(sequence))
    {
        if (m_Stream >> next)
        {
            buffer += next;
        }
        else
        {
            Reset(position);
            return false;
        }
    }

    value = buffer;
    return true;
}

bool Tokenizer::NextSequence(const std::string& sequence)
{
    size_t position = m_Stream.tellg();

    for (char expected : sequence)
    {
        char actual;
        if (m_Stream >> actual)
        {
            if (expected != actual)
            {
                Reset(position);
                return false;
            }
        }

        if (m_Stream.bad())
        {
            Reset(position);
            return false;
        }
    }

    return true;
}

bool Tokenizer::NextWord(const std::string& word)
{
    size_t position = m_Stream.tellg();

    // Match with word
    if (NextSequence(word))
    {
        // Check that next character is not alphanumeric
        if (!std::isalnum(m_Stream.peek()))
        {
            Reset(position);
            return false;
        }

        // Check that next character is not '_'
        if (std::char_traits<char>::to_char_type(m_Stream.peek()) == '_')
        {
            Reset(position);
            return false;
        }

        //Expectation valid
        return true;
    }

    return false;
}

bool Tokenizer::NextComment(Token& token)
{
    //Single Line Comment
    if (NextSequence("//"))
    {
        std::string value;
        if (ReadUntilSequence("\n", value))
        {
            token = CommentToken{ value };
        }
        return true;
    }

    //Multi Line Comment
    if (NextSequence("/*"))
    {
        std::string value;
        if (ReadUntilSequence("*/", value))
        {
            token = CommentToken{ value };
            return true;
        }
    }

    return false;
}

bool Tokenizer::NextBooleanLiteral(Token& token)
{
    if (NextWord("true"))
    {
        token = BooleanLiteralToken{ true };
        return true;
    }
    
    if (NextWord("false"))
    {
        token = BooleanLiteralToken{ false };
        return true;
    }

    return false;
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
    size_t position = m_Stream.tellg();

    char next;
    std::string value;

    if (m_Stream >> next && next == '"')
    {
        while (m_Stream >> next)
        {
            if (next == '\\') // Escape character
            {
                m_Stream >> next;
                value += next;
                continue;
            }

            if (next == '"') // End of String
            {
                token = StringLiteralToken{ value };
                return true;
            }

            // Buffer string
            value += next;
        }

        Reset(position);
        return false;
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
    if (!std::isalpha(c) || c == '_') return false;

    //Buffer the identifier
    while (m_Stream.good() && (std::isalnum(m_Stream.peek()) || std::char_traits<char>::to_char_type(m_Stream.peek()) == '_'))
    {
        m_Stream.get(c);
        value += c;
    }

    token = IdentifierToken{ value };
    return true;
}

bool Tokenizer::NextSymbol(Token& token)
{
    if (NextSequence("<<")) { token = SymbolToken{ "<<" }; return true; }
    if (NextSequence(">>")) { token = SymbolToken{ ">>" }; return true; }

    if (NextSequence("!=")) { token = SymbolToken{ "!=" }; return true; }
    if (NextSequence("==")) { token = SymbolToken{ "==" }; return true; }
    if (NextSequence(">=")) { token = SymbolToken{ ">=" }; return true; }
    if (NextSequence("<=")) { token = SymbolToken{ "<=" }; return true; }
    
    if (NextSequence("+=")) { token = SymbolToken{ "+=" }; return true; }
    if (NextSequence("-=")) { token = SymbolToken{ "-=" }; return true; }
    if (NextSequence("*=")) { token = SymbolToken{ "*=" }; return true; }
    if (NextSequence("/=")) { token = SymbolToken{ "/=" }; return true; }
    if (NextSequence("^=")) { token = SymbolToken{ "^=" }; return true; }
    if (NextSequence("&=")) { token = SymbolToken{ "&=" }; return true; }
    if (NextSequence("|=")) { token = SymbolToken{ "|=" }; return true; }
    if (NextSequence("~=")) { token = SymbolToken{ "~=" }; return true; }
    if (NextSequence("%=")) { token = SymbolToken{ "%=" }; return true; }
    
    if (NextSequence("&&")) { token = SymbolToken{ "&&" }; return true; }
    if (NextSequence("||")) { token = SymbolToken{ "||" }; return true; }

    if (NextSequence("::")) { token = SymbolToken{ "::" }; return true; }

    std::string value;

    if (Read(value, 1))
    {
        token = SymbolToken{ value };
        return true;
    }

    return false;
}