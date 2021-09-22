#pragma once

#include <string>
#include <vector>

#include "HeaderParserLib/Token.h"
#include "HeaderParserLib/Tokenizer.h"
#include "HeaderParserLib/Metadata/Namespace.h"
#include "HeaderParserLib/Metadata/Metadata.h"

struct ParserConfiguration
{
    std::string ClassAnnotationIdentifier       = "CLASS";
    std::string PropertyAnnotationIdentifier    = "PROPERTY";
    std::string FunctionAnnotationIdentifier    = "FUNCTION";
};


class Parser
{
public:
    Parser();
    void Parse(const std::string& source);

    void ParseStatement();

    void ParseNamespace();
    void ParseClass();
    void ParseProperty();

    std::string ParseType();
    std::string ParseTypeDeclarator();

    void SkipScope();
    void SkipToSymbol(const std::string& value);

    void ParseMetadata(Metadata& metadata);

    inline ParserConfiguration& GetParserConfiguration() { return m_Configuration; }
    inline const Namespace& GetGblobalNamespace() const { return m_GlobalNamespace; }

private:
    inline Class* GetCurrentClass() const { return m_ClassStack.back(); }
    inline void PushClass(Class* clazz) { m_ClassStack.push_back(clazz); }
    inline void PopClass() { m_ClassStack.pop_back(); }

    inline Namespace* GetCurrentNamespace() const { return m_NamespaceStack.back(); }
    inline void PushNamespace(Namespace* nameSpace) { m_NamespaceStack.push_back(nameSpace); }
    inline void PopNamespace() { m_NamespaceStack.pop_back(); }

private:
    Tokenizer m_Tokenizer;
    ParserConfiguration m_Configuration;

    Namespace m_GlobalNamespace;

    std::vector<Namespace*> m_NamespaceStack;
    std::vector<Class*> m_ClassStack;
};