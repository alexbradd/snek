#include "./input.h"

static void handle_sig_int(int sig);

#if defined(_WIN64) || defined(_WIN32) // OS - Windows

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

static DWORD initial_mode_in = 0,
             initial_mode_out = 0;

static HANDLE h_stdout = INVALID_HANDLE_VALUE,
              h_stdin = INVALID_HANDLE_VALUE;

int init_term(void)
{
    DWORD mode_in, mode_out;

    h_stdin = GetStdHandle(STD_INPUT_HANDLE);
    h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_stdin == INVALID_HANDLE_VALUE || h_stdout == INVALID_HANDLE_VALUE)
        return GetLastError();

    if (!GetConsoleMode(h_stdin, &mode_in) || !GetConsoleMode(h_stdout, &mode_out))
        return GetLastError();
    memcpy(&initial_mode_in, &mode_in, sizeof(DWORD));
    memcpy(&initial_mode_out, &mode_out, sizeof(DWORD));

    mode_out |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    mode_in |= ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!SetConsoleMode(h_stdout, mode_out) || !SetConsoleMode(h_stdin, mode_in))
        return GetLastError();

    signal(SIGINT, handle_sig_int);

    return 0;
}

int reset_term(void)
{  
    if (h_stdin == INVALID_HANDLE_VALUE || h_stdout == INVALID_HANDLE_VALUE)
        return 1;

    if (initial_mode_in == 0 || initial_mode_out == 0)
        return 1;

    if (!SetConsoleMode(h_stdout, initial_mode_out) || !SetConsoleMode(h_stdin, initial_mode_in))
        return GetLastError();
    return 0;
}

int getch(void)
{
    int read;
    INPUT_RECORD inp;
    do {
        ReadConsoleInput(h_stdin, &inp, 1, &read);
    } while (inp.Event.KeyEvent.bKeyDown == FALSE || inp.Event.KeyEvent.uChar.AsciiChar == '\0');
    return inp.Event.KeyEvent.uChar.AsciiChar;
}

int kbhit(void)
{
    int read;
    INPUT_RECORD inp;

    PeekConsoleInput(h_stdin, &inp, 1, &read);
    if (read >= 1) {
        if (inp.Event.KeyEvent.bKeyDown == FALSE || inp.Event.KeyEvent.uChar.AsciiChar == '\0') {
            ReadConsoleInput(h_stdin, &inp, 1, &read); // discard useless events to avoid reading the same event forever
            return kbhit(); // retest for good mesure
        }
        return 1;
    }
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
    return sigaction(SIGQUIT, &sig, NULL);
}

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

static void handle_sig_int(int sig)
{
    exit(reset_term());
}
