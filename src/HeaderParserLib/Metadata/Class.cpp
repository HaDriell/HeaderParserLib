#include "HeaderParserLib/Metadata/Class.h"


Class::Class(const std::string& name)
    : m_Name(name)
{
}


Field* Class::AddField(const std::string& type, const std::string& name)
{
    return &m_Fields.emplace_back(type, name);
}


Function* Class::AddMethod(const std::string& returnType, const std::string& name, const std::vector<Argument>& arguments)
{
    return &m_Methods.emplace_back(returnType, name, arguments);
}