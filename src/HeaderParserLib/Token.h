#pragma once

#include <cstdint>
#include <variant>
#include <string>


enum class TokenType
{
    Invalid = 0,
    Whitespace,
    Comment,
    Symbol,
    Identifier,
    
    Count
};

struct Token
{
    TokenType   Type{ TokenType::Invalid };
    std::string Value;
};