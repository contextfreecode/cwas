#include "field.c"

const char* approved = "";

void constrained_init(int argc, char** argv) {
    // Super init then custom.
    init(argc, argv);
    assert(argc > 3);
    approved = argv[3];
}
