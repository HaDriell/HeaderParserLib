#include "HeaderParserLib/Parser.h"

#include <algorithm>
#include <exception>

Parser::Parser()
    : m_GlobalNamespace("")
{
}


void Parser::SetSource(const std::string& source)
{
    //Initialize Tokenizer
    m_Tokenizer.SetSource(source);

    //Clear Stacks
    m_NamespaceStack.clear();
    m_ClassStack.clear();

    //Prepare for a new Parsing
    m_GlobalNamespace = Namespace("");
    m_NamespaceStack.push_back(&m_GlobalNamespace);
}


void Parser::ParseProgram()
{
    while (ParseStatement())
    {
    }
}


bool Parser::ParseStatement()
{
    if (ParseNamespace()) return true;

    return false;
}


bool Parser::ParseNamespace()
{
    if (!m_Tokenizer.ExpectIdentifier("namespace")) return false;

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
        if (!ParseStatement()) throw std::exception("Failed to Parse Statement");
    }

    if (ns !=nullptr)
    {
        PopNamespace();
    }

    return true;
}

bool Parser::ParseClass()
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
    return true;
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


bool Parser::ParseType(std::string& type)
{
    //Match prefix cv specifiers
    std::vector<std::string> prefixes;
    while (true)
    {
        if (m_Tokenizer.ExpectIdentifier("const"))      { prefixes.push_back("const"); continue; }
        if (m_Tokenizer.ExpectIdentifier("volatile"))   { prefixes.push_back("volatile"); continue; }
        break;
    }

    std::string declarator = ParseTypeDeclarator();

    //Resolve Template Parameters
    std::vector<std::string> templateParameters;
    if (ParseTemplateParameters(templateParameters))
    {
        declarator += "<";
        bool first = true;
        for (const std::string& tparam : templateParameters)
        {
            if (first) first = false;
            else declarator += ", ";
            declarator += tparam;
        }
        declarator += ">";
    }

    std::vector<std::string> suffixes;

    //Check for pointer and/or reference suffixes
    while (true)
    {
        if (m_Tokenizer.ExpectIdentifier("const")) { suffixes.push_back("const"); continue; }
        if (m_Tokenizer.ExpectSymbol("&&")) { suffixes.push_back("&&"); continue; }
        if (m_Tokenizer.ExpectSymbol("*"))  { suffixes.push_back("*"); continue; }
        if (m_Tokenizer.ExpectSymbol("&"))  { suffixes.push_back("&"); continue; }
        break;
    }

    //Add prefixes
    {
        std::reverse(prefixes.begin(), prefixes.end());
        for (const std::string& prefix : prefixes)
        {
            declarator = prefix + " " + declarator;
        }
    }

    //Add Suffixes
    {
        for (const std::string& suffix : suffixes)
        {
            declarator += " " + suffix;
        }
    }

    //TODO : append suffixes
    type = declarator;
    return true;
}


std::string Parser::ParseTypeDeclarator()
{
    //Ignore optional forward declaration specifiers
    m_Tokenizer.ExpectIdentifier("class");
    m_Tokenizer.ExpectIdentifier("struct");
    m_Tokenizer.ExpectIdentifier("typename");

    //Parse type name
    bool first = true;
    std::string declarator;
    while (true)
    {
        if (m_Tokenizer.ExpectSymbol("::"))
        {
            declarator += "::";
        }
        else if (!first) // required to correctly parse global namespace resolver prefixing a type
        {
            break;
        }

        first = false;

        Token token;
        if (!m_Tokenizer.GetIdentifier(token)) throw std::exception("Unexpected Token");
        declarator += token.Value;
    }

    return declarator;
}


bool Parser::ParseTemplateParameters(std::vector<std::string>& parameters)
{
    if (!m_Tokenizer.ExpectSymbol("<")) return false; // Not a Template
    if (m_Tokenizer.ExpectSymbol(">")) return true; // Empty Template

    //Parse Parameter Pack
    while (true)
    {
        std::string type;
        if (!ParseType(type)) throw std::exception("Unexpected Token");
        parameters.push_back(type);
        if (m_Tokenizer.ExpectSymbol(",")) continue;
        break;
    }
    
    if (!m_Tokenizer.ExpectSymbol(">")) throw std::exception("Unexpected Token");
    return true;
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