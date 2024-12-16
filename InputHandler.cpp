#include "InputHandler.hpp"

using namespace std;

bool InputHandler::checkDoublePress(char input) {
    auto now = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::duration<double>>(
        now - lastInputTime).count();
    
    bool isDouble = false;
    if (duration < DOUBLE_PRESS_THRESHOLD && !modeBuffer.empty() && 
        modeBuffer.back() == input) {
        isDouble = true;
        modeBuffer.clear();
    } else {
        modeBuffer = input;
    }
    
    lastInputTime = now;
    return isDouble;
}

bool InputHandler::checkSequence(const string& input, const string& sequence) {
    modeBuffer += input;
    if (modeBuffer.length() > sequence.length()) {
        modeBuffer = modeBuffer.substr(1);
    }
    return modeBuffer == sequence;
}