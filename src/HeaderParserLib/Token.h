#pragma once

#include <cstdint>
#include <string>
#include <string_view>

enum class TokenType
{
    Unknown,
    Symbol,
    Identifier,
    Constant,
};

struct Token
{
    TokenType           m_Type;
    std::string_view    m_Value;
};