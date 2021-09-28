#include "HeaderParserLib/Metadata/Annotation.h"


Annotation::Annotation(const std::string& name)
    : m_Name(name)
{
}


bool Annotation::HasAttribute(const std::string& attribute)
{
    auto it = m_Attributes.find(attribute);
    return it != m_Attributes.end();
}


bool Annotation::GetAttribute(const std::string& attribute, std::string& value) const
{
    auto it = m_Attributes.find(attribute);
    if (it != m_Attributes.end())
    {
        value = it->second;
        return true;
    }
    return false;
}


bool Annotation::SetAttribute(const std::string& attribute, const std::string& value)
{
    auto it = m_Attributes.find(attribute);
    if (it == m_Attributes.end())
    {
        m_Attributes[attribute] = value;
        return true;
    }

    return false;
}