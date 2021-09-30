#pragma once

#include <string>
#include <vector>
#include <map>

#include "HeaderParserLib/Metadata/Field.h"
#include "HeaderParserLib/Metadata/Function.h"
#include "HeaderParserLib/Metadata/Annotation.h"

class Class
{
public:
    Class(const std::string& name);

    inline const std::string& GetName() const { return m_Name; }

    inline const std::vector<std::string>& GetParents() const { return m_Parents; }
    inline void SetParents(const std::vector<std::string>& parents) { m_Parents = parents; }

    inline const std::map<std::string, Field>& GetFields() const { return m_Fields; }
    const Field* GetField(const std::string& name) const;
    Field* AddField(const std::string& type, const std::string& name);

    inline const std::vector<Function>& GetMethods() const { return m_Methods; }
    Function* AddMethod(const std::string& returnType, const std::string& name, const std::vector<Argument>& arguments);

    inline const std::vector<Annotation>& GetAnnotations() const { return m_Annotations; }
    inline void SetAnnotations(const std::vector<Annotation>& annotations) { m_Annotations = annotations; }

private:
    std::string m_Name;
    std::vector<std::string> m_Parents;
    std::map<std::string, Field> m_Fields;
    std::vector<Function> m_Methods;
    std::vector<Annotation> m_Annotations;
};