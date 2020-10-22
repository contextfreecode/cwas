#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Ensure we don't lose our 
#ifdef NDEBUG
#   define check(x) (x)
#else
#   define check(x) assert(x)
#endif

size_t max_size;
const char* label = "";
struct termios old_termios;
size_t size = 0;
char* value = "";

void draw(void);
void finally(void);
bool keep_char(int c);

void cancel(void) {
    value[(size = 0)] = 0;  // Wild and crazy assignment expression.
    draw();
    fprintf(stderr, COLOR_CANCEL "-" COLOR_RESET);
}

void catch_abort(int signum) {
    finally();
    signal(SIGABRT, SIG_DFL);
    raise(SIGABRT);
}

void catch_signal(int signum) {
    cancel();
    exit(EXIT_FAILURE);
}

void draw(void) {
    fprintf(stderr, ERASE_LINE "\r");
    fprintf(stderr, COLOR_LABEL "%s:" COLOR_RESET " %s", label, value);
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
    assert(argc > 2);
    label = argv[1];
    max_size = atoll(argv[2]);
    check((value = malloc(max_size + 1)));
    memset(value, 0, max_size);
    // Prepare terminal.
    check(!tcgetattr(STDIN_FILENO, &old_termios));
    signal(SIGABRT, catch_abort);
    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ECHO | ICANON);
    check(!tcsetattr(STDIN_FILENO, TCSANOW, &new_termios));
    // assert(false);
    // Handle cleanup.
    check(!atexit(finally));
    signal(SIGINT, catch_signal);
}

void prompt() {
    while (true) {
        draw();
        handle_char();
    }
}
