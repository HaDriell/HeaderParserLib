#include "HeaderParserLib/Metadata/Function.h"


Argument::Argument(const std::string& type, const std::string& name)
    : m_Type(type)
    , m_Name(name)
{
}

Function::Function(const std::string& name, const std::string& returnType, const std::vector<Argument>& arguments)
    : m_Name(name)
    , m_ReturnType(returnType)
    , m_Arguments(arguments)
{
}
