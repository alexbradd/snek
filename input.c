#include "./input.h"

#if defined(_WIN64) || defined(_WIN32) // OS - Windows

#include <windows.h>
#include <stdio.h>
#include <string.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

static DWORD initial_mode = 0;

int init_term(void)
{
    HANDLE h_stdout;
    DWORD mode;

    h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_stdout == INVALID_HANDLE_VALUE)
        return GetLastError();

    if (!GetConsoleMode(h_stdout, &mode))
        return GetLastError();
    memcpy(&initial_mode, &mode, sizeof(DWORD));

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(h_stdout, mode))
        return GetLastError();

    return 0;
}

int reset_term(void)
{
    HANDLE h_stdout;

    if (initial_mode == 0)
        return 1;

    h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_stdout == INVALID_HANDLE_VALUE)
        return GetLastError();

    if (!SetConsoleMode(h_stdout, initial_mode))
        return GetLastError();
    return 0;
}

#elif defined(__unix__) || defined(__APPLE__) // OS - Unix

#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>
#include <termios.h>
#include <string.h>

#include <signal.h>

static struct termios orig_termios = {};
static void handle_sig_int(int sig);

int init_term(void)
{
    int exit;
    struct termios new_termios;
    struct sigaction sig;

    exit = tcgetattr(STDIN_FILENO, &orig_termios);
    if (exit != 0)
        return exit;
    memcpy(&new_termios, &orig_termios, sizeof(struct termios));

    cfmakeraw(&new_termios);
    new_termios.c_iflag &= ~IGNBRK;
    new_termios.c_iflag |= BRKINT;
    new_termios.c_lflag |= ISIG;
    exit = tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    if (exit != 0)
        return exit;

    sig.sa_handler = handle_sig_int;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;
    sigaction(SIGQUIT, &sig, NULL);

    return 0;
}

static void handle_sig_int(int sig)
{ reset_term(); exit(0); }

int reset_term(void)
{
    struct termios empty = {};
    if (orig_termios == empty)
        return 1;
    return tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

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
