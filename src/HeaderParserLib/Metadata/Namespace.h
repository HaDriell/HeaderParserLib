#pragma once

#include <string>
#include <map>

#include "HeaderParserLib/Metadata/Class.h"

class Namespace
{
public:
    Namespace(const std::string& name);

    inline const std::string& GetName() const { return m_Name; }

    inline const std::map<std::string, Class>& GetClasses() const { return m_Classes; }
    Class* GetClass(const std::string& name);
    Class* AddClass(const std::string& name);

    inline const std::map<std::string, Namespace>& GetNamespaces() const { return m_Namespaces; }
    Namespace* GetNamespace(const std::string& name);
    Namespace* AddNamespace(const std::string& name);

private:
    std::string m_Name;
    std::map<std::string, Class> m_Classes;
    std::map<std::string, Namespace> m_Namespaces;
};