#include "HeaderParserLib/Metadata/Metadata.h"


bool Metadata::Get(const std::string& attribute, std::string& value) const
{
    auto it = m_Metadatas.find(attribute);
    if (it != m_Metadatas.end())
    {
        value = it->second;
        return true;
    }
    return false;
}

bool Metadata::Set(const std::string& attribute, const std::string& value)
{
    auto it = m_Metadatas.find(attribute);
    if (it == m_Metadatas.end())
    {
        m_Metadatas[attribute] = value;
        return true;
    }

    return false;
}