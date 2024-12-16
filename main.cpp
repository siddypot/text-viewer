#include "Editor.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }
    
    Editor editor(argv[1]);
    editor.run();
    
    return 0;
}
