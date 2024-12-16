#include "Editor.hpp"
#include "FileHandler.hpp"
#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <thread>

using namespace std;

namespace {
    struct termios orig_termios;

    void enableRawMode() {
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    void disableRawMode() {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }
}
bool Editor::loadFile() {
    buffer.clear(); // Clear existing buffer
    return FileHandler::readFile(filename, buffer);
}

bool Editor::saveFile() const {
    return FileHandler::writeFile(filename, buffer);
}

Editor::Editor(const string& fname) 
    : filename(fname), 
      currentMode(Mode::VISUAL),
      lastBlink(chrono::steady_clock::now()) {
    if (!loadFile()) {
        buffer.push_back("");
    }
    enableRawMode();
    atexit(disableRawMode);
}

void Editor::switchMode(Mode newMode) {
    currentMode = newMode;
    inputHandler.clearBuffer();
}

void Editor::updateCursorBlink() {
    auto now = chrono::steady_clock::now();
    if (now - lastBlink >= BLINK_INTERVAL) {
        cursor.toggleVisibility();
        lastBlink = now;
    }
}

string Editor::getBufferLineWithCursor(size_t lineNum) const {
    if (lineNum != cursor.getRow() || !cursor.isVisible()) {
        return buffer[lineNum];
    }
    
    string line = buffer[lineNum];
    if (cursor.getCol() <= line.length()) {
        line.insert(cursor.getCol(), 1, cursor.getSymbol());
    }
    return line;
}

void Editor::displayBuffer() const {
    system("clear");
    for (size_t i = 0; i < buffer.size(); ++i) {
        cout << getBufferLineWithCursor(i) << "\n";
    }
}

void Editor::displayStatus() const {
    cout << "\n--- " 
         << (currentMode == Mode::INSERT ? "INSERT" : "VISUAL") 
         << " --- Line: " << cursor.getRow() + 1 
         << "/" << buffer.size() 
         << " Col: " << cursor.getCol() + 1 << " ---\n"
         << "Commands: q(quit) s(save) n(new line) d(delete line) ss(insert) sSs(visual)\n";
}

void Editor::moveCursor(int rowDelta, int colDelta) {
    // Update row
    int newRow = static_cast<int>(cursor.getRow()) + rowDelta;
    if (newRow >= 0 && newRow < static_cast<int>(buffer.size())) {
        cursor.setPosition(newRow, cursor.getCol());
    }
    
    // Update column
    int newCol = static_cast<int>(cursor.getCol()) + colDelta;
    if (newCol >= 0 && newCol <= static_cast<int>(buffer[cursor.getRow()].length())) {
        cursor.setPosition(cursor.getRow(), newCol);
    }
}

void Editor::processArrowKey() {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return;
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return;

    if (seq[0] == '[') {
        switch (seq[1]) {
            case 'A': moveCursor(-1, 0); break; 
            case 'B': moveCursor(1, 0); break;  
            case 'C': moveCursor(0, 1); break;  
            case 'D': moveCursor(0, -1); break;
        }
    }
}

void Editor::processCommand(char cmd) {
    if (cmd == '\x1b') { // ESC key
        if (currentMode == Mode::INSERT) {
            switchMode(Mode::VISUAL);
            return;
        }
        processArrowKey();
        return;
    }

    // Check for mode switches
    if (cmd == 's') {
        if (inputHandler.checkDoublePress('s')) {
            switchMode(Mode::INSERT);
            return;
        }
        if (inputHandler.checkSequence("s", "sSs")) {
            switchMode(Mode::VISUAL);
            return;
        }
    }

    switch (cmd) {
        case 'q':
            cout << "Save before quit? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y') {
                saveFile();
            }
            disableRawMode();
            exit(0);
            break;
            
        case 's':
            if (currentMode == Mode::VISUAL) {
                if (saveFile()) {
                    cout << "File saved successfully!\n";
                } else {
                    cout << "Error saving file!\n";
                }
            }
            break;
            
        case 'n':
            if (currentMode == Mode::INSERT) {
                buffer.insert(buffer.begin() + cursor.getRow() + 1, "");
                moveCursor(1, 0);
            }
            break;
            
        case 'd':
            if (currentMode == Mode::VISUAL && buffer.size() > 1) {
                buffer.erase(buffer.begin() + cursor.getRow());
                if (cursor.getRow() >= buffer.size()) {
                    moveCursor(-1, 0);
                }
            }
            break;
            
        default:
            if (currentMode == Mode::INSERT && cmd >= 32 && cmd < 127) {
                string& line = buffer[cursor.getRow()];
                line.insert(cursor.getCol(), 1, cmd);
                moveCursor(0, 1);
            }
    }
}

void Editor::run() {
    char input;
    while (true) {
        updateCursorBlink();
        displayBuffer();
        displayStatus();
        
        fd_set fds;
        struct timeval tv;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 100000; //timeout

        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
            if (read(STDIN_FILENO, &input, 1) == 1) {
                processCommand(input);
            }
        }
    }
}