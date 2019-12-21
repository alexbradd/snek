#include "./input.h"

#if defined(__unix__) || defined(__APPLE__) // OS - Unix

#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>
#include <termios.h>
#include <string.h>

#include <signal.h>

static struct termios orig_termios;

static void handle_sig_int(int sig);

void init_term(void)
{
    struct termios new_termios;
    struct sigaction sig;

    tcgetattr(STDIN_FILENO, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(struct termios));

    cfmakeraw(&new_termios);
    new_termios.c_iflag &= ~IGNBRK;
    new_termios.c_iflag |= BRKINT;
    new_termios.c_lflag |= ISIG;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    sig.sa_handler = handle_sig_int;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;
    sigaction(SIGQUIT, &sig, NULL);
}

static void handle_sig_int(int sig)
{ reset_term(); exit(0); }

void reset_term(void)
{ tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios); }

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

#endif // OS
