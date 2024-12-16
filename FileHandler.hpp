#pragma once
#include <string>
#include <vector>

class FileHandler {
public:
    static bool readFile(const std::string& filename, std::vector<std::string>& buffer);
    static bool writeFile(const std::string& filename, const std::vector<std::string>& buffer);
};
