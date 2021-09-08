#pragma once

#include <cstdint>
#include <variant>
#include <string>

struct StringLiteralToken
{
    std::string Value;
};

struct IntegerLiteralToken
{
    int64_t Value;
};

struct IdentifierToken
{
    std::string Value;
};

using Token = std::variant<
    StringLiteralToken,
    IntegerLiteralToken,
    IdentifierToken
>;

class TokenHandler
{
public:
    TokenHandler();
    virtual ~TokenHandler();

    void HandleToken(const Token& token);
    
protected:
    virtual void Handle(const StringLiteralToken& token) {}
    virtual void Handle(const IntegerLiteralToken& token) {}
    virtual void Handle(const IdentifierToken& token) {}

};