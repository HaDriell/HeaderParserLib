#include "HeaderParserLib/Metadata/Namespace.h"
#include <cassert>

Namespace::Namespace(const std::string& name)
    : m_Name(name)
{
}


const Class* Namespace::GetClass(const std::string& name) const
{
    auto it = m_Classes.find(name);
    if (it != m_Classes.end())
    {
        return &it->second;
    }

    return nullptr;
}


Class* Namespace::AddClass(const std::string& name)
{
    assert(m_Classes.find(name) == m_Classes.end());
    m_Classes.insert({name, Class(name)});
    return &m_Classes.find(name)->second;
}


const Namespace* Namespace::GetNamespace(const std::string& name) const
{
    auto it = m_Namespaces.find(name);
    if (it != m_Namespaces.end())
    {
        return &it->second;
    }

    return nullptr;
}


Namespace* Namespace::AddNamespace(const std::string& name)
{
    if (m_Namespaces.find(name) == m_Namespaces.end())
    {
        m_Namespaces.insert({name, Namespace(name)});
    }

    return &m_Namespaces.find(name)->second;
}
