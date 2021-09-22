#pragma once

#include <string>
#include <map>

class Metadata
{
public:
    bool Get(const std::string& attribute, std::string& value) const;
    bool Set(const std::string& attribute, const std::string& value);

    inline const std::map<std::string, std::string>& All() const { return m_Metadatas; }

private:
    std::map<std::string, std::string> m_Metadatas;
};