#include "field.c"

// Keep out of lib for overriding.
bool keep_char(int c) {
    return true;
}

int main(int argc, char** argv) {
    init(argc, argv);
    prompt();
}
