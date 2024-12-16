#pragma once
#include <string>

using namespace std;

enum class Mode {
    VISUAL,
    INSERT
};

class EditorMode {
public:
    virtual ~EditorMode() = default;
    virtual void handleInput(char input) = 0;
    virtual string getModeName() const = 0;
};