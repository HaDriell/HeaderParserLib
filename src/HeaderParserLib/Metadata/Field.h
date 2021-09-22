#pragma once

#include <string>

#include "HeaderParserLib/Metadata/Metadata.h"



class Field
{
public:
    Field(const std::string& type, const std::string& name);

    inline const std::string& GetType() const { return m_Type; }
    inline const std::string& GetName() const { return m_Name; }

    inline const Metadata& GetMetadata() const { return m_Metadata; }
    inline void SetMetadata(const Metadata& metadata) { m_Metadata = metadata; }
private:
    std::string m_Type;
    std::string m_Name;
    Metadata m_Metadata;
};