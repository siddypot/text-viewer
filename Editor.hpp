#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Cursor.hpp"
#include "InputHandler.hpp"
#include "EditorMode.hpp"

using namespace std;

class Editor {
private:
    vector<string> buffer;
    string filename;
    Cursor cursor;
    Mode currentMode;
    InputHandler inputHandler;
    chrono::steady_clock::time_point lastBlink;
    const chrono::milliseconds BLINK_INTERVAL{500};
    
    void displayBuffer() const;
    void displayStatus() const;
    void processCommand(char cmd);
    void processArrowKey();
    bool saveFile() const;
    bool loadFile();
    void moveCursor(int rowDelta, int colDelta);
    void switchMode(Mode newMode);
    void updateCursorBlink();
    string getBufferLineWithCursor(size_t lineNum) const;

public:
    Editor(const string& fname);
    void run();
};