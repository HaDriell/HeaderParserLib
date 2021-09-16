#include "HeaderParserLib/Metadata/Namespace.h"

Namespace::Namespace(const std::string& name)
    : m_Name(name)
{
}


Class* Namespace::AddClass(const std::string& name)
{
    return &m_Classes.emplace_back(name);
}


Namespace* Namespace::AddNamespace(const std::string& name)
{
    return &m_Namespaces.emplace_back(name);  
}
