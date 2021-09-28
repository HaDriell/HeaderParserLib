#pragma once

#include <string>
#include <vector>

#include "HeaderParserLib/Metadata/Annotation.h"



class Field
{
public:
    Field(const std::string& type, const std::string& name);

    inline const std::string& GetType() const { return m_Type; }
    inline const std::string& GetName() const { return m_Name; }

    inline const std::vector<Annotation>& GetAnnotations() const { return m_Annotations; }
    inline void SetAnnotations(const std::vector<Annotation>& annotations) { m_Annotations = annotations; }

private:
    std::string m_Type;
    std::string m_Name;

    std::vector<Annotation> m_Annotations;
};