#pragma once

#include <string>

class Field
{
public:
    Field(const std::string& type, const std::string& name);

    inline const std::string& GetType() const { return m_Type; }
    inline const std::string& GetName() const { return m_Name; }
private:
    std::string m_Type;
    std::string m_Name;
};