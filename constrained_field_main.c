#include <string.h>
#include "constrained_field.c"

bool keep_char(int c) {
    return !!strchr(approved, c);
}

int main(int argc, char** argv) {
    constrained_init(argc, argv);
    prompt();
}
