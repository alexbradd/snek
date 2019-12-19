#include "./input.h"

#if defined(__unix__) || defined(__APPLE__)

void init_term(struct termios *orig_termios)
{
    struct termios new_termios;

    tcgetattr(STDIN_FILENO, orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(struct termios));

    cfmakeraw(&new_termios);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void reset_term(struct termios *orig_termios)
{ tcsetattr(STDIN_FILENO, TCSANOW, orig_termios); }

int kbhit(void)
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch(void)
{
    int r;
    unsigned char c;
    if ((r = read(STDIN_FILENO, &c, sizeof(c)) < 0))
        return r;
    else
        return c;
}

#endif
