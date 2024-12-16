#pragma once
#include <cstddef>

using namespace std;

class Cursor {
private:
    size_t row;
    size_t col;
    bool visible;
    const char CURSOR_SYMBOL = '|';

public:
    Cursor() : row(0), col(0), visible(true) {}
    void move(int rowDelta, int colDelta);
    void toggleVisibility() { visible = !visible; }
    bool isVisible() const { return visible; }
    size_t getRow() const { return row; }
    size_t getCol() const { return col; }
    void setPosition(size_t newRow, size_t newCol) {
        row = newRow;
        col = newCol;
    }
    char getSymbol() const { return CURSOR_SYMBOL; }
};