#include "FileHandler.hpp"
#include <fstream>

bool FileHandler::readFile(const std::string& filename, std::vector<std::string>& buffer) {
    std::ifstream file(filename);
    if (!file) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        buffer.push_back(line);
    }
    
    return true;
}

bool FileHandler::writeFile(const std::string& filename, const std::vector<std::string>& buffer) {
    std::ofstream file(filename);
    if (!file) {
        return false;
    }
    
    for (const auto& line : buffer) {
        file << line << "\n";
    }
    
    return true;
}
