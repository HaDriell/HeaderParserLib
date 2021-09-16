#include "HeaderParserLib/Parser.h"


Parser::Parser()
{
}


bool Parser::Parse(const std::string& source)
{
    //Clear Stacks
    m_NamespaceStack.clear();
    m_ClassStack.clear();

    //Initialize State
    m_Tokenizer.SetSource(source);
    m_GlobalNamespace = Namespace();
    m_NamespaceStack.push_back(&m_GlobalNamespace);
    
    while (ParseStatement())
    {
    }

    return true;
}


bool Parser::ParseStatement()
{
    Token token;
    if (m_Tokenizer.GetToken(token))
    {
        return ParseDeclaration(token);        
    }

    return false;
}


bool Parser::ParseDeclaration(const Token& token)
{
    switch (token.Type)
    {
    case TokenType::Identifier:
        if (token.Value == "CLASS") return ParseClass();
        if (token.Value == "PROPERTY") return ParseProperty();
        if (token.Value == "FUNCTION") return ParseFunction();
        break;

    default:
        break;
    }

    return true;
}

bool Parser::ParseNamespace()
{
    // Expect the namespace keyword
    if (!m_Tokenizer.ExpectIdentifier("namespace")) return false;

    // Get the <name> of the namespace
    Token namespaceNameToken;
    if (!m_Tokenizer.GetIdentifier(namespaceNameToken)) return false;

    if (!m_Tokenizer.ExpectSymbol("{")) return false;

    PushNamespace( GetCurrentNamespace()->AddNamespace(namespaceNameToken.Value) );

    while (!m_Tokenizer.ExpectSymbol("}") && ParseStatement())
    {
    }

    PopNamespace();

    return false;
}

bool Parser::ParseClass()
{
    if (!ParseMetadata()) return false;

    //Expect 'class' or 'struct'
    if (!m_Tokenizer.ExpectIdentifier("class") && !m_Tokenizer.ExpectIdentifier("struct")) return false;

    //Read the ClassName
    Token classNameToken;
    if (!m_Tokenizer.GetIdentifier(classNameToken)) return false;
    //We try to avoid capturing dll export macros there, we should hit a Symbol ':' or '{' at some point
    while (m_Tokenizer.GetIdentifier(classNameToken))
    {
    }

    //MAYDO : Parse Inheritance tree ?

    //Skip to the Class Scope
    if (!m_Tokenizer.SkipToSymbol("{")) return false;

    PushClass( GetCurrentNamespace()->AddClass(classNameToken.Value) );

    while (!m_Tokenizer.ExpectSymbol("}") && ParseStatement())
    {
    }

    PopClass();

    return true;
}


bool Parser::ParseProperty()
{
    if (!ParseMetadata()) return false;

    Token propertyTypeName;
    if (!m_Tokenizer.GetIdentifier(propertyTypeName)) return false;

    Token propertyName;
    if (!m_Tokenizer.GetIdentifier(propertyName)) return false;

    //MAYDO : Parse initialization expression ?

    if (!m_Tokenizer.SkipToSymbol(";")) return false;

    return true;
}


bool Parser::ParseFunction()
{
    if (!ParseMetadata()) return false;

    Token returnTypeName;
    if (!m_Tokenizer.GetIdentifier(returnTypeName)) return false;

    Token functionName;
    if (!m_Tokenizer.GetIdentifier(functionName)) return false;

    if (!m_Tokenizer.SkipToSymbol(";")) return false;

    return true;
}


bool Parser::ParseMetadata()
{
    if (!m_Tokenizer.ExpectSymbol("(")) return false;

    // Parse Metadata here instead of Skipping

    if (!m_Tokenizer.SkipToSymbol(")")) return false;

    return true;
}