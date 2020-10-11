#include "field.c"

const char* approved = "";

void constrained_init(int argc, char** argv) {
    assert(argc > 3);
    approved = argv[3];
    init(argc, argv);
}
