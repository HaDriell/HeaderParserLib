#pragma once

#include <string>
#include <vector>

#include "HeaderParserLib/Token.h"
#include "HeaderParserLib/Tokenizer.h"
#include "HeaderParserLib/Metadata/Namespace.h"


class Parser
{
public:
    Parser();
    bool Parse(const std::string& source);

    bool ParseStatement();
    bool ParseDeclaration(const Token& token);

    bool ParseNamespace();
    bool ParseClass();
    bool ParseProperty();
    bool ParseFunction();

    bool ParseMetadata();

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
    Namespace m_GlobalNamespace;

    std::vector<Namespace*> m_NamespaceStack;
    std::vector<Class*> m_ClassStack;
};