#include "HeaderParserLib/Tokenizer.h"

#include <algorithm>
#include <regex>
#include <map>
#include <cstdarg>


void Tokenizer::SetSource(const std::string& source)
{
    m_Stream = std::istringstream(source);
    m_Stream >> std::noskipws;
}


size_t Tokenizer::GetPosition()
{
    return m_Stream.tellg();
}


void Tokenizer::SetPosition(size_t position, bool clearErrors)
{
    if (clearErrors)
    {
        m_Stream.clear();
    }
    m_Stream.seekg(position);
}


bool Tokenizer::Get(char& character)
{
    m_Stream >> character;    
    return m_Stream.good(); // Reached EOF ?! WTF
}


bool Tokenizer::GetString(std::string& value, size_t count)
{
    value.resize(count);
    for (char& character : value)
    {
        if (!Get(character))
        {
            return false;
        }
    }
    
    return true;
}


bool Tokenizer::GetString(std::string& value, const std::string& delimiter)
{
    std::string buffer;
    char character;

    while (Get(character))
    {
        buffer += character;

        // match <delimiter> to the end of <buffer>
        if (buffer.size() >= delimiter.size())
        {
            auto eos_index = buffer.size() - delimiter.size();
            if (buffer.compare(eos_index, delimiter.size(), delimiter.data()) == 0)
            {
                value = buffer.substr(0, eos_index);
                return true;
            }
        }
    }

    return false;
}


bool Tokenizer::ExpectString(const std::string& expected)
{
    size_t position = GetPosition();

    std::string actual;
    if (GetString(actual, expected.size()) && actual == expected)
    {
        return true;
    }

    SetPosition(position);
    return false;
}


bool Tokenizer::GetWord(std::string& value)
{
    if (std::isalpha(m_Stream.peek()) || std::char_traits<char>::to_char_type(m_Stream.peek()) == '_')
    {
        char character;
        std::string buffer;
        while (std::isalnum(m_Stream.peek()) || std::char_traits<char>::to_char_type(m_Stream.peek()) == '_')
        {
            if (m_Stream >> character)
            {
                buffer += character;
            }
            else
            {
                return false; // mostlikely reached EOF ?! WTF
            }
        }
        value = buffer;
        return true;
    }

    return false;
}


bool Tokenizer::ExpectWord(const std::string& expected)
{
    size_t position = GetPosition();
    
    std::string actual;
    if (GetWord(actual) && expected == actual)
    {
        return true;
    }

    SetPosition(position);
    return false;
}


void Tokenizer::SkipWhitespaces()
{
    while (std::isspace(m_Stream.peek()))
    {
        m_Stream.get();
    }
}


bool Tokenizer::GetComment(Token& token)
{
    size_t position = GetPosition();
    SkipWhitespaces();

    //Single Line Comment
    if (ExpectString("//"))
    {
        std::string value;
        if (GetString(value, "\n")) // ends at new-line
        {
            token.Type = TokenType::Comment;
            token.Value = value;
            return true;
        }
    }

    //Multi Line Comment
    if (ExpectString("/*"))
    {
        std::string value;
        if (GetString(value, "*/")) // ends at */
        {
            token.Type = TokenType::Comment;
            token.Value = value;
            return true;
        }
    }

    SetPosition(position);
    return false;
}


bool Tokenizer::GetIdentifier(Token& token)
{
    size_t position = GetPosition();
    SkipWhitespaces();

    if (GetWord(token.Value))
    {
        token.Type = TokenType::Identifier;
        return true;
    }
    
    SetPosition(position);
    return false;
}


bool Tokenizer::GetSymbol(Token& token)
{
    size_t position = GetPosition();
    SkipWhitespaces();

    /* Test for known double character operators */

    std::string op;
    if (op = "::"; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "->"; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }

    if (op = ">>"; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "<<"; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }

    if (op = "!="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "=="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = ">="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "<="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }

    if (op = "+="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "-="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "*="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "/="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "^="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "~="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "&="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "|="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "%="; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }

    if (op = "&&"; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "||"; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }

    if (op = "++"; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }
    if (op = "--"; ExpectString(op)) { token.Type = TokenType::Symbol; token.Value = op; return true; }


    //TODO : Remove when I know how to better how to NOT accept unknown symbols
    char symbol;
    if (Get(symbol))
    {
        token.Type = TokenType::Symbol;
        token.Value = std::string(1, symbol);
        return true;
    }

    SetPosition(position);
    return false;
}


bool Tokenizer::GetToken(Token& token)
{
    SkipWhitespaces();
    
    if (GetComment(token)) return true;
    if (GetIdentifier(token)) return true;
    if (GetSymbol(token)) return true;
    return false;
}


bool Tokenizer::ExpectIdentifier(const std::string& expected)
{
    size_t position = GetPosition();

    Token token;
    if (GetIdentifier(token) && token.Value == expected)
    {
        return true;
    }

    SetPosition(position);
    return false;
}


bool Tokenizer::ExpectSymbol(const std::string& expected)
{
    size_t position = GetPosition();

    Token token;
    if (GetSymbol(token) && token.Value == expected)
    {
        return true;
    }

    SetPosition(position);
    return false;
}