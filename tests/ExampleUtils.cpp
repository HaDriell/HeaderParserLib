#include "ExampleUtils.h"

void LoadFile(const std::filesystem::path& filepath, std::string& content)
{
    std::ifstream file(filepath.u8string().c_str());
    content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}