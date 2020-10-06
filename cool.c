// #include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// C was already sharp

struct termios old_termios;

void finally() {
    // endwin();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    printf("\nDone\n");
}

void handle_signal(int signum) {
    exit(EXIT_FAILURE);
}

int main() {
    tcgetattr(STDIN_FILENO, &old_termios);
    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    // filter();
    // initscr();
    atexit(finally);
    signal(SIGINT, handle_signal);
    // cbreak();
    // printw("Hi!");
    // refresh();
    // int c = getch();
    int c;
    while (true) {
        c = getchar();
        if (c == '\n') {
            break;
        }
        printf("\r%x", c);
    }
}
