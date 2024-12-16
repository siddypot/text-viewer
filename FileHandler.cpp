#include "FileHandler.hpp"
#include <fstream>

using namespace std;

bool FileHandler::readFile(const string& filename, vector<string>& buffer) {
    ifstream file(filename);
    if (!file) {
        return false;
    }
    
    string line;
    while (getline(file, line)) {
        buffer.push_back(line);
    }
    
    return true;
}

bool FileHandler::writeFile(const string& filename, const vector<string>& buffer) {
    ofstream file(filename);
    if (!file) {
        return false;
    }
    
    for (const auto& line : buffer) {
        file << line << "\n";
    }
    
    return true;
}