#include "Editor.hpp"
#include "FileHandler.hpp"
#include <iostream>
#include <cstdlib>

Editor::Editor(const std::string& fname) : filename(fname), cursorRow(0), cursorCol(0) {
    if (!loadFile()) {
        buffer.push_back("");
    }
}

bool Editor::loadFile() {
    return FileHandler::readFile(filename, buffer);
}

bool Editor::saveFile() const {
    return FileHandler::writeFile(filename, buffer);
}

void Editor::displayBuffer() const {
    system("clear"); // Use "cls" for Windows
    for (size_t i = 0; i < buffer.size(); ++i) {
        std::cout << buffer[i] << "\n";
    }
}

void Editor::displayStatus() const {
    std::cout << "\n--- Line: " << cursorRow + 1 << "/" << buffer.size() 
              << " Col: " << cursorCol + 1 << " ---\n"
              << "Commands: q(quit) s(save) n(new line) d(delete line)\n";
}

void Editor::processCommand(char cmd) {
    switch (cmd) {
        case 'q':
            std::cout << "Save before quit? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y') {
                saveFile();
            }
            exit(0);
            break;
            
        case 's':
            if (saveFile()) {
                std::cout << "File saved successfully!\n";
            } else {
                std::cout << "Error saving file!\n";
            }
            break;
            
        case 'n':
            buffer.insert(buffer.begin() + cursorRow + 1, "");
            cursorRow++;
            cursorCol = 0;
            break;
            
        case 'd':
            if (buffer.size() > 1) {
                buffer.erase(buffer.begin() + cursorRow);
                if (cursorRow >= buffer.size()) {
                    cursorRow = buffer.size() - 1;
                }
            }
            break;
            
        default:
            if (cmd != '\n') {
                buffer[cursorRow].insert(cursorCol, 1, cmd);
                cursorCol++;
            }
    }
}

void Editor::run() {
    char input;
    while (true) {
        displayBuffer();
        displayStatus();
        
        input = std::cin.get();
        processCommand(input);
    }
}
