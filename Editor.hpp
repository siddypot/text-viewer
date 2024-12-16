#pragma once
#include <string>
#include <vector>

class Editor {
private:
    std::vector<std::string> buffer;
    std::string filename;
    size_t cursorRow;
    size_t cursorCol;
    
    void displayBuffer() const;
    void displayStatus() const;
    void processCommand(char cmd);
    bool saveFile() const;
    bool loadFile();

public:
    Editor(const std::string& fname);
    void run();
};
