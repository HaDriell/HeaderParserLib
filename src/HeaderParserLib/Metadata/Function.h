#pragma once

#include <string>
#include <vector>

#include "HeaderParserLib/Metadata/Annotation.h"

class Argument
{
public:
    Argument(const std::string& type, const std::string& name);

    inline const std::string& GetType() const { return m_Type; }
    inline const std::string& GetName() const { return m_Name; }

private:
    std::string m_Type;
    std::string m_Name;
};

class Function
{
public:
    Function(const std::string& returnType, const std::string& name, const std::vector<Argument>& arguments);

    inline const std::string& GetReturnType() const { return m_ReturnType; }
    inline const std::string& GetName() const { return m_Name; }
    inline const std::vector<Argument>& GetArguments() const { return m_Arguments; }
    
    inline const std::vector<Annotation>& GetAnnotations() const { return m_Annotations; }
    inline void SetAnnotations(const std::vector<Annotation>& annotations) { m_Annotations = annotations; }
    
private:
    std::string m_ReturnType;
    std::string m_Name;
    std::vector<Argument> m_Arguments;
    std::vector<Annotation> m_Annotations;
};