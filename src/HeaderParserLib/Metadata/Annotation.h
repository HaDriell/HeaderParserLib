#pragma once

#include <string>
#include <map>

class Annotation
{
public:
    Annotation(const std::string& name);

    bool HasAttribute(const std::string& attribute);
    bool GetAttribute(const std::string& attribute, std::string& value) const;
    bool SetAttribute(const std::string& attribute, const std::string& value);

    inline const std::map<std::string, std::string>& GetAttributes() const { return m_Attributes; }
private:
    std::string m_Name;
    std::map<std::string, std::string> m_Attributes;
};