#include "HeaderParserLib/Tokenizer.h"

Tokenizer::Tokenizer(std::istream& stream)
    : m_Stream(stream)
{
}

Tokenizer::~Tokenizer()
{
}

void Tokenizer::Reset()
{
    m_Stream.clear();
    m_Stream.seekg(0, std::ios::beg);
}

char Tokenizer::GetChar()
{
    return m_Stream.get();
}

char Tokenizer::PeekChar()
{
    return m_Stream.peek();
}

void Tokenizer::UngetChar()
{
    m_Stream.seekg(m_Stream.tellg(), std::ios::beg);
}

bool Tokenizer::IsEOF()
{
    return m_Stream.eof();
}

bool Tokenizer::NextToken()
{
    char current = GetChar();
    char next = PeekChar();

    bool hasNextToken = false;
    while (!hasNextToken)
    {
        switch (current)
        {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            // Whitespace
            break;
        
        default:
            break;
        }
    }
    

    return false;
}