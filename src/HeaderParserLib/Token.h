#pragma once

#include <cstdint>
#include <variant>
#include <string>

struct SymbolToken
{
    std::string Value;
};

struct CommentToken
{
    std::string Value;
};

struct BooleanLiteralToken
{
    bool Value;
};

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
    SymbolToken,
    CommentToken,
    BooleanLiteralToken,
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
    virtual void Handle(const SymbolToken& token) {}
    virtual void Handle(const CommentToken& token) {}
    virtual void Handle(const BooleanLiteralToken& token) {}
    virtual void Handle(const StringLiteralToken& token) {}
    virtual void Handle(const IntegerLiteralToken& token) {}
    virtual void Handle(const IdentifierToken& token) {}
};