#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define CHAR_BACKSPACE '\x7f'
#define CHAR_ENTER '\n'
#define CHAR_ESCAPE '\x1b'
#define COLOR_CANCEL "\x1b[91m"
#define COLOR_LABEL "\x1b[94m"
#define COLOR_RESET "\x1b[39;49m"
#define ERASE_LINE "\x1b[2K"
#define EXIT_CANCEL 2

size_t max_size;
const char* label = "";
struct termios old_termios;
size_t size = 0;
char* value = "";

bool keep_char(int c);

void draw(void) {
    fprintf(stderr, ERASE_LINE "\r");
    fprintf(stderr, COLOR_LABEL "%s:" COLOR_RESET " %s", label, value);
}

void cancel(void) {
    value[size = 0] = 0;  // Wild and crazy assignment expression.
    draw();
    fprintf(stderr, COLOR_CANCEL "-" COLOR_RESET);
}

void catch_signal(int signum) {
    cancel();
    exit(EXIT_FAILURE);
}

void finally(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    fprintf(stderr, "\n");
    printf("%s\n", value);
}

void handle_char(void) {
    int c = getchar();
    switch (c) {
        case CHAR_BACKSPACE:
            if (size) {
                value[--size] = 0;  // Wild and crazy pre decrement.
            }
            break;
        case CHAR_ENTER:
            if (*value) {
                exit(EXIT_SUCCESS);
            }
            break;
        case CHAR_ESCAPE:
            cancel();
            exit(EXIT_CANCEL);
        default:
            if (size < max_size && keep_char(c)) {
                value[size++] = c;  // Wild and crazy post increment.
                value[size] = 0;
            }
            break;
    }
}

void init(int argc, char** argv) {
    // signal(SIGABRT, catch_signal);
    assert(argc > 2);
    label = argv[1];
    max_size = atoll(argv[2]);
    assert((value = malloc(max_size + 1)));
    // Prepare terminal.
    assert(!tcgetattr(STDIN_FILENO, &old_termios));
    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ECHO | ICANON);
    assert(!tcsetattr(STDIN_FILENO, TCSANOW, &new_termios));
    // Handle cleanup.
    assert(!atexit(finally));
    signal(SIGINT, catch_signal);
}

void prompt() {
    draw();
    while (true) {
        handle_char();
        draw();
    }
}
