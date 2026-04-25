#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <poll.h>

#include "irobot.h"


struct termios orig_termios;

// Restore terminal to its original state on exit
void reset_terminal_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

// Disable line buffering and echoing
void set_conio_terminal_mode() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(reset_terminal_mode);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

int main() {
    set_conio_terminal_mode();

    struct pollfd pfd = { .fd = STDIN_FILENO, .events = POLLIN };
    char c;

    printf("Press keys (Press 'q' to quit)...\n");


    int res = irobot_init("/dev/ttyUSB0");
    if ( res < 0 ) {
        return -1;
    }

    int16_t speed = 0;
    int16_t angle = 0;

    irobot_start_full();

    while (1) {
        // Poll for 100ms. Returns > 0 if a key was pressed.
        int ret = poll(&pfd, 1, 100);

        if (ret > 0 && (pfd.revents & POLLIN)) {
            if (read(STDIN_FILENO, &c, 1) > 0) {
                if (c == 'q') {
                    break;
                }

                if ( c == 'w' ) {
                    irobot_move(100, 0);

                } else if ( c == 's' ) {
                    irobot_move(-100, 0);

                } else if ( c == 'a' ) {
                    irobot_move(100, 100);

                } else if ( c == 'd' ) {
                    irobot_move(100, -100);

                } else if ( c == ' ' ) {
                    irobot_move(0, 0);

                } else {
                    printf("Key pressed: %c\n", c);
                    fflush(stdout);
                }
            }
        }
    }

    // FIM
    irobot_move_stop();
    irobot_close();
    return 0;
}
