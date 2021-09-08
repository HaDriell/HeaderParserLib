#include "HeaderParserLib/Token.h"

TokenHandler::TokenHandler()
{
}

TokenHandler::~TokenHandler()
{
}

void TokenHandler::HandleToken(const Token& token)
{
    std::visit([this](const auto& t) { Handle(t); }, token);
}