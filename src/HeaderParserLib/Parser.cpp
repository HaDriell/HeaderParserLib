#include "HeaderParserLib/Parser.h"

#include <exception>

Parser::Parser()
    : m_GlobalNamespace("")
{
}


void Parser::Parse(const std::string& source)
{
    //Clear Stacks
    m_NamespaceStack.clear();
    m_ClassStack.clear();

    //Initialize State
    m_Tokenizer.SetSource(source);
    m_GlobalNamespace = Namespace("");
    m_NamespaceStack.push_back(&m_GlobalNamespace);
    
    while (!m_Tokenizer.IsEOF())
    {
        ParseStatement();
    }
}


void Parser::ParseStatement()
{
    Token token;
    if (m_Tokenizer.GetToken(token))
    {
        switch (token.Type)
        {
        case TokenType::Identifier:
            if (token.Value == "namespace") ParseNamespace();
            if (token.Value == "CLASS") ParseClass();
            if (token.Value == "PROPERTY") ParseProperty();
            break;

        default:
            break;
        }
    }
}


void Parser::ParseNamespace()
{
    /// namespace <Identifier> { ... }
    ///           ^
    ///           We are here

    // find the Namespace identifier or inline the namespace
    Token nameToken;
    Namespace* ns = m_Tokenizer.GetIdentifier(nameToken) ? GetCurrentNamespace()->AddNamespace(nameToken.Value) : nullptr;

    if (ns != nullptr)
    {
        PushNamespace(ns);
    }

    if (!m_Tokenizer.ExpectSymbol("{")) throw std::exception("Unexpected Token");
    while (!m_Tokenizer.ExpectSymbol("}"))
    {
        ParseStatement();
    }

    if (ns !=nullptr)
    {
        PopNamespace();
    }
}

void Parser::ParseClass()
{
    Metadata metadata;
    ParseMetadata(metadata);

    //Expect 'class' or 'struct'
    if (!m_Tokenizer.ExpectIdentifier("class") && !m_Tokenizer.ExpectIdentifier("struct")) throw std::exception("Unexpected Token");

    //Read the ClassName
    //Look for the ClassName. It's the last Identifier found before Symbols ':' or '{'
    Token classNameToken;
    if (!m_Tokenizer.GetIdentifier(classNameToken)) throw std::exception("Unexpected Token");
    while (m_Tokenizer.GetIdentifier(classNameToken))
    {
    }

    if (m_Tokenizer.ExpectSymbol(":"))
    {
        //TODO : Parse inheritance declarations
    }
    
    //In any case skip until we reach the class scope
    SkipToSymbol("{");

    Class* clazz = GetCurrentNamespace()->AddClass(classNameToken.Value);
    clazz->SetMetadata(metadata);

    PushClass(clazz);

    while (!m_Tokenizer.ExpectSymbol("}"))
    {
        ParseStatement();
    }

    PopClass();
}


void Parser::ParseProperty()
{
    Metadata metadata;
    ParseMetadata(metadata);
    
    Token propertyTypeName;
    if (!m_Tokenizer.GetIdentifier(propertyTypeName)) throw std::exception("Unexpected Token");

    Token propertyName;
    if (!m_Tokenizer.GetIdentifier(propertyName)) throw std::exception("Unexpected Token");

    SkipToSymbol(";"); // Don't care about the initializations for now

    Field* field = GetCurrentClass()->AddField(propertyTypeName.Value, propertyName.Value);
    field->SetMetadata(metadata);
}


std::string Parser::ParseType()
{
    //Match prefix keywords (static, mutable, const)
    bool isMutable = false;
    bool isConst = false;
    bool isVolatile = false;
    while (true)
    {
        if (m_Tokenizer.ExpectIdentifier("mutable"))    { isMutable = true; continue; }
        if (m_Tokenizer.ExpectIdentifier("const"))      { isConst = true; continue; }
        if (m_Tokenizer.ExpectIdentifier("volatile"))   { isVolatile = true; continue; }
        break;
    }

    std::string declarator = ParseTypeDeclarator();

    //Postfix 'const' specifier
    isConst |= m_Tokenizer.ExpectIdentifier("const");

    //Template ?
    if (m_Tokenizer.ExpectSymbol("<"))
    {
        std::vector<std::string> arguments;
        if (!m_Tokenizer.ExpectSymbol(">"))
        {
            do {
                arguments.push_back( ParseType() );
            } while (m_Tokenizer.ExpectSymbol(","));

            if (!m_Tokenizer.ExpectSymbol(">")) throw std::exception("Unexpected Token");
        }

        //TODO : append declarator template arguments
    }

    //Check for pointer and/or reference suffixes
    while (true)
    {
        if (m_Tokenizer.ExpectSymbol("*"))  { declarator += "*"; continue; }
        if (m_Tokenizer.ExpectSymbol("&"))  { declarator += "&"; continue; }
        if (m_Tokenizer.ExpectSymbol("&&")) { declarator += "&&"; continue; }
        break;
    }

    //Function pointer ?
    // if (m_Tokenizer.ExpectSymbol("("))
    // {
    //     if (m_Tokenizer.ExpectSymbol("*"))
    //     {
    //         if (!m_Tokenizer.ExpectSymbol(")"))
    //         {

    //         }
    //     }
    // }

    //TODO : append suffixes

    return declarator;
}


std::string Parser::ParseTypeDeclarator()
{
    //Ignore optional forward declaration specifiers
    m_Tokenizer.ExpectIdentifier("class");
    m_Tokenizer.ExpectIdentifier("struct");
    m_Tokenizer.ExpectIdentifier("typename");

    std::string declarator;
    bool lastIsNamespace = false;

    //Check for a potential global Namesapce prefix
    if (m_Tokenizer.ExpectSymbol("::"))
    {
        declarator += "::";
        lastIsNamespace = true;
    }
    

    //Parse type name
    while (true)
    {
        if (lastIsNamespace)
        {
            Token token;
            if (!m_Tokenizer.GetIdentifier(token)) throw std::exception("Unexpected Token");

            declarator += token.Value;
            lastIsNamespace = false;
            continue;
        }
        
        if (!lastIsNamespace)
        {
            if (!m_Tokenizer.ExpectIdentifier("::")) break;
            
            declarator += "::";
            lastIsNamespace = true;
        }
    }

    //Check for invalid type declarators (ending with a namespace resolver or just empty)
    if (declarator.empty() || declarator.back() == ':') throw std::exception("Unexpected Token");

    return declarator;
}


void Parser::SkipScope()
{
    uint32_t depth = 1;
    Token token;

    while (m_Tokenizer.GetToken(token))
    {
        if (token.Type == TokenType::Symbol)
        {
            if (token.Value == "{")
            {
                depth++;
            }

            if (token.Value == "}")
            {
                depth--;
            }

            if (depth == 0)
            {
                return;
            }
        }
    }

    throw std::exception("Unexpected End of Stream");
}


void Parser::SkipToSymbol(const std::string& value)
{
    Token token;
    while (m_Tokenizer.GetToken(token))
    {
        if (token.Type == TokenType::Symbol && token.Value == value)
        {
            break;
        }
    }
}


void Parser::ParseMetadata(Metadata& metadata)
{
    if (!m_Tokenizer.ExpectSymbol("(")) throw std::exception("Unexpected Token");
    if (!m_Tokenizer.ExpectSymbol(")"))
    {
        do
        {
            //Match an identifier
            Token keyToken;
            Token valueToken;
            if (!m_Tokenizer.GetIdentifier(keyToken)) throw std::exception("Unexpected Token");
            if (!m_Tokenizer.ExpectSymbol("=")) throw std::exception("Unexpected Token");
            if (!m_Tokenizer.GetIdentifier(valueToken)) throw std::exception("Unexpected Token");

            metadata.Set(keyToken.Value, valueToken.Value);

        } while (m_Tokenizer.ExpectSymbol(","));

        if (!m_Tokenizer.ExpectSymbol(")")) throw std::exception("Unexpected Token");
    }

}