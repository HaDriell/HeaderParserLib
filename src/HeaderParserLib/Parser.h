#pragma once

#include <string>
#include <vector>

#include "HeaderParserLib/Token.h"
#include "HeaderParserLib/Tokenizer.h"
#include "HeaderParserLib/Metadata/Namespace.h"
#include "HeaderParserLib/Metadata/Annotation.h"

struct ParserConfiguration
{
    std::vector<std::string> Annotations = { "CLASS", "PROPERTY", "FUNCTION" };
};


class Parser
{
public:
    Parser();
    void SetSource(const std::string& source);

    void ParseProgram();
    bool ParseStatement();

    // Standalone Statements

    bool ParseNamespace();
    bool ParseClass();
    bool ParseField();
    bool SkipDeclaration();

    // Utility Parsing Functions & Dependent Statements

    bool ParseAnnotation(std::vector<Annotation>& annotations);

    bool ParseType(std::string& type);
    bool ParseTemplateParameters(std::vector<std::string>& parameters);
    std::string ParseTypeDeclarator();

    inline ParserConfiguration& GetParserConfiguration() { return m_Configuration; }
    inline const Namespace& GetGblobalNamespace() const { return m_GlobalNamespace; }

private:
    inline Class* GetCurrentClass() const { return m_ClassStack.empty() ? nullptr : m_ClassStack.back(); }
    inline void PushClass(Class* clazz) { m_ClassStack.push_back(clazz); }
    inline void PopClass() { m_ClassStack.pop_back(); }

    inline Namespace* GetCurrentNamespace() const { return m_NamespaceStack.empty() ? nullptr : m_NamespaceStack.back(); }
    inline void PushNamespace(Namespace* nameSpace) { m_NamespaceStack.push_back(nameSpace); }
    inline void PopNamespace() { m_NamespaceStack.pop_back(); }


private:
    Tokenizer m_Tokenizer;
    ParserConfiguration m_Configuration;

    Namespace m_GlobalNamespace;

    std::vector<Namespace*> m_NamespaceStack;
    std::vector<Class*> m_ClassStack;
};