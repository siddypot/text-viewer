#pragma once
#include <string>
#include <chrono>

using namespace std;

class InputHandler {
private:
    string modeBuffer;
    chrono::steady_clock::time_point lastInputTime;
    const double DOUBLE_PRESS_THRESHOLD = 0.3; // seconds

public:
    InputHandler() : lastInputTime(chrono::steady_clock::now()) {}
    bool checkDoublePress(char input);
    bool checkSequence(const string& input, const string& sequence);
    void clearBuffer() { modeBuffer.clear(); }
};