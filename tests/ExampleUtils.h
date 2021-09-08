#pragma once

#include <filesystem>
#include <fstream>
#include <string>

void LoadFile(const std::filesystem::path& filepath, std::string& content);
