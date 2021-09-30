#include "HeaderParserLib/Metadata/Class.h"


Class::Class(const std::string& name)
    : m_Name(name)
{
}

const Field* Class::GetField(const std::string& name) const
{
    auto it = m_Fields.find(name);
    if (it != m_Fields.end())
    {
        return &it->second;
    }
    return nullptr;
}

Field* Class::AddField(const std::string& type, const std::string& name)
{
    if (m_Fields.find(name) == m_Fields.end())
    {
        m_Fields.insert({name, Field(type, name)});
    }
    return &m_Fields.find(name)->second;
}


Function* Class::AddMethod(const std::string& returnType, const std::string& name, const std::vector<Argument>& arguments)
{
    return &m_Methods.emplace_back(returnType, name, arguments);
}