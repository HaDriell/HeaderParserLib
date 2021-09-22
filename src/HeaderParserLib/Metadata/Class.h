#pragma once

#include <string>
#include <vector>
#include <map>

#include "HeaderParserLib/Metadata/Field.h"
#include "HeaderParserLib/Metadata/Function.h"
#include "HeaderParserLib/Metadata/Metadata.h"

class Class
{
public:
    Class(const std::string& name);

    inline const std::string& GetName() const { return m_Name; }

    inline const std::vector<Field>& GetFields() const { return m_Fields; }
    Field* AddField(const std::string& type, const std::string& name);

    inline const std::vector<Function>& GetMethods() const { return m_Methods; }
    Function* AddMethod(const std::string& returnType, const std::string& name, const std::vector<Argument>& arguments);

    inline const Metadata& GetMetadata() const { return m_Metadata; }
    inline void SetMetadata(const Metadata& metadata) { m_Metadata = metadata; }

private:
    std::string m_Name;
    std::vector<Field> m_Fields;
    std::vector<Function> m_Methods;
    Metadata m_Metadata;
};