#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// C was already sharp

#define COLOR_PROMPT "\x1b[92m"
#define COLOR_RESET "\x1b[39;49m"
#define ERASE_LINE "\x1b[2K"

struct termios old_termios;
size_t max_size;
const char* prompt;
char* value = NULL;

void catch_signal(int signum) {
    exit(EXIT_FAILURE);
}

void finally(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    fprintf(stderr, "\nDone\n");
}

void init(void) {
    assert((value = malloc(max_size)));
    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    atexit(finally);
    signal(SIGINT, catch_signal);
}

void write_prompt(void) {
    fprintf(stderr, ERASE_LINE "\r");
    if (*prompt) {
        fprintf(stderr, COLOR_PROMPT "%s" COLOR_RESET " ", prompt);
    }
}

int main(int argc, char** argv) {
    tcgetattr(STDIN_FILENO, &old_termios);
    signal(SIGABRT, catch_signal);
    assert(argc > 1);
    max_size = atoi(argv[1]);
    prompt = argc > 2 ? argv[2] : "";
    init();
    write_prompt();
    int c;
    while (true) {
        c = getchar();
        if (c == '\n') {
            break;
        }
        write_prompt();
        fprintf(stderr, "%x", c);
    }
}
