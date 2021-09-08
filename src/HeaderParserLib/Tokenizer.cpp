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
    size_t position = m_Stream.tellg();

    // Check for Comments
    if (NextComment(token))                 return true; else Reset(position);
    
    // Check for Constants & Literals
    if (NextIntegerLiteral(token))          return true; else Reset(position);
    if (NextStringLiteral(token))           return true; else Reset(position);
    if (NextBooleanLiteral(token))          return true; else Reset(position);
    
    // Check for identifiers
    if (NextIdentifier(token))              return true; else Reset(position);
    
    // Check for symbols
    if (NextSymbol(token))                  return true; else Reset(position);

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

bool Tokenizer::Expect(const std::string& sequence)
{
    size_t position = m_Stream.tellg();
    std::string actual;
    actual.resize(sequence.size());

    if (m_Stream.read(actual.data(), actual.size()))
    {
        return sequence == actual;
    }
    
    return false;
}

bool Tokenizer::ExpectWord(const std::string& word)
{
    //No match
    if (!Expect(word)) return false;
    //Stream is still eating a word. word was only a part of the match
    if (std::isalnum(m_Stream.peek())) return false;
    if (std::char_traits<char>::to_char_type(m_Stream.peek()) == '_') return false;
    return true;
}

bool Tokenizer::ReadUntilExpect(const std::string& sequence, std::string& value)
{
    std::string buffer = "";
    while (!Expect(sequence))
    {
        char c;
        if (m_Stream >> c)
        {
            buffer += c;
        }
        else
        {
            return false;
        }
    }

    value = buffer;
    return true;
}

bool Tokenizer::NextComment(Token& token)
{
    //Single Line Comment
    if (Expect("//"))
    {
        std::string value;
        if (ReadUntilExpect("\n", value))
        {
            token = CommentToken{ value };
        }
        return true;
    }

    //Multi Line Comment
    if (Expect("/*"))
    {
        std::string value;
        if (ReadUntilExpect("*/", value)))
        {
            token = CommentToken{ value };
            return true;
        }
    }

    return false;
}


bool Tokenizer::NextBooleanLiteral(Token& token)
{
    if (ExpectWord("true"))
    {
        token = BooleanLiteralToken{ true };
        return true;
    }
    
    if (ExpectWord("false"))
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
    char delimiter;
    if (!m_Stream.get(delimiter)) return false;

    //Not a String Literal
    if (delimiter != '\'' && delimiter != '"')
    {
        return false;
    }

    std::string value;

    char next;
    while (m_Stream.get(next))
    {
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
    std::string value;
    value.resize(2, ' ');

    if (!m_Stream.read(value.data(), value.length()))
    {
        return false;
    }

    if (value != "<<"
    &&  value != ">>" //Might be a problem for templates
    
    &&  value != "!="
    &&  value != "=="
    &&  value != ">="
    &&  value != "<="
    
    &&  value != "+="
    &&  value != "-="
    &&  value != "*="
    &&  value != "/="
    &&  value != "^="
    &&  value != "&="
    && value != "|="
    &&  value != "~="
    &&  value != "%="

    &&  value != "&&"
    &&  value != "||"

    &&  value != "::")
    {
        m_Stream.unget();
        value.resize(1);
    }
    
    token = SymbolToken{ value };
    return true;
}