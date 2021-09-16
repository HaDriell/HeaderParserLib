#pragma once

#include <string>
#include <vector>

#include "HeaderParserLib/Metadata/Class.h"

class Namespace
{
public:
    Namespace(const std::string& name = "");

    inline const std::string& GetName() const { return m_Name; }

    inline const std::vector<Class>& GetClasses() const { return m_Classes; }
    Class* AddClass(const std::string& name);

    inline const std::vector<Namespace>& GetNamespaces() const { return m_Namespaces; }
    Namespace* AddNamespace(const std::string& name);

private:
    std::string m_Name;
    std::vector<Class> m_Classes;
    std::vector<Namespace> m_Namespaces;
};