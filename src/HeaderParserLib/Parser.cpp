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
    if (ParseClass()) return true;
    if (ParseField()) return true;

    return SkipDeclaration();
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

    if (ns != nullptr)
    {
        PopNamespace();
    }

    return true;
}


bool Parser::ParseClass()
{
    size_t position = m_Tokenizer.GetPosition();

    std::vector<Annotation> annotations;
    while (ParseAnnotation(annotations))
    {
    }

    //Expect 'class' or 'struct' to begin parsing a class
    if (!m_Tokenizer.ExpectIdentifier("class") && !m_Tokenizer.ExpectIdentifier("struct"))
    {
        m_Tokenizer.SetPosition(position);
        return false;
    }

    //Read the ClassName
    //Look for the ClassName. It's the last Identifier found before Symbols ':' or '{'
    Token classNameToken;
    if (!m_Tokenizer.GetIdentifier(classNameToken)) throw std::exception("Unexpected Token");
    while (m_Tokenizer.GetIdentifier(classNameToken))
    {
    }

    //That's a forward declaration !
    if (m_Tokenizer.ExpectSymbol(";"))
    {
        if (!annotations.empty()) throw std::exception("Annotations on Forward Declarations is not supported.");
        return true;
    }

    std::vector<std::string> parents;
    if (m_Tokenizer.ExpectSymbol(":"))
    {
        while (true)
        {
            std::string type;
            if (!ParseType(type)) throw std::exception("Unexpected Token");
            parents.push_back(type);

            if (m_Tokenizer.ExpectSymbol(",")) continue;
            else break;
        }
        //TODO : Parse inheritance declarations
    }
    
    //In any case skip until we reach the class scope
    if (!m_Tokenizer.ExpectSymbol("{")) throw std::exception("Unexpected Token");

    Class* clazz = GetCurrentNamespace()->AddClass(classNameToken.Value);
    clazz->SetParents(parents);
    clazz->SetAnnotations(annotations);

    PushClass(clazz);

    while (!m_Tokenizer.ExpectSymbol("}"))
    {
        ParseStatement();
    }

    PopClass();
    return true;
}


bool Parser::ParseField()
{
    Class* clazz = GetCurrentClass();
    if (clazz == nullptr) return false; // Not in a Class

    size_t position = m_Tokenizer.GetPosition();

    std::vector<Annotation> annotations;
    while (ParseAnnotation(annotations))
    {
    }

    //Consume prefixes
    bool isStatic = false;
    bool isMutable = false;
    while (true)
    {
        if (m_Tokenizer.ExpectIdentifier("static"))     { isStatic = true; continue; }
        if (m_Tokenizer.ExpectIdentifier("mutable"))    { isMutable = true; continue; }
        break;
    }

    std::string type;
    if (!ParseType(type))
    {
        m_Tokenizer.SetPosition(position);
        return false;
    }

    Token fieldNameToken;
    if (!m_Tokenizer.GetIdentifier(fieldNameToken)) throw std::exception("Unexpected Token");

    //TODO : parse raw array expressions here

    // Skip end of statement (possible affectations, initialization scopes, constructor, ...)
    if (!SkipDeclaration()) throw std::exception("Unexpected End of Stream");

    Field* field = clazz->AddField(type, fieldNameToken.Value);
    field->SetAnnotations(annotations);
    
    return true;
}


bool Parser::SkipDeclaration()
{
    size_t scopeDepth = 0;
    Token token;
    while (m_Tokenizer.GetToken(token))
    {
        if (token.Type == TokenType::Symbol)
        {
            if (scopeDepth == 0 && token.Value == ";") return true;
            else if (token.Value == "{") scopeDepth++;
            else if (token.Value == "}") scopeDepth--;
        }
    }

    return false;
}


bool Parser::ParseAnnotation(std::vector<Annotation>& annotations)
{
    size_t position = m_Tokenizer.GetPosition();

    //Annotations must start with an identifier
    Token annotationToken;
    if (!m_Tokenizer.GetIdentifier(annotationToken)) return false;

    // Validate that the Annotation identifier has been declared
    auto it = std::find(m_Configuration.Annotations.begin(), m_Configuration.Annotations.end(), annotationToken.Value);
    if (it == m_Configuration.Annotations.end())
    {
        m_Tokenizer.SetPosition(position);
        return false;
    }
    
    Annotation& annotation = annotations.emplace_back(annotationToken.Value);

    if (!m_Tokenizer.ExpectSymbol("(")) throw std::exception("Unexpected Token");
    if (m_Tokenizer.ExpectSymbol(")")) return true;

    while (true)
    {
        Token attributeNameToken;
        Token attributeValueToken;
        //Require the attribute name
        if (!m_Tokenizer.GetIdentifier(attributeNameToken)) throw std::exception("Unexpected Token");
        //Optionaly require the attribute value
        if (m_Tokenizer.ExpectSymbol("="))
        {
            if (!m_Tokenizer.GetIdentifier(attributeValueToken)) throw std::exception("Unexpected Token");
        }

        annotation.SetAttribute(attributeNameToken.Value, attributeValueToken.Value);

        if (m_Tokenizer.ExpectSymbol(",")) continue;
        else break;
    }

    if (!m_Tokenizer.ExpectSymbol(")")) throw std::exception("Unexpected Token");

    return true;
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
        else break;
    }
    
    if (!m_Tokenizer.ExpectSymbol(">")) throw std::exception("Unexpected Token");
    return true;
}