#ifndef __INPUT_H__
#   define __INPUT_H__

#   define UP_K    'w'
#   define DOWN_K  's'
#   define RIGHT_K 'd'
#   define LEFT_K  'a'
#   define QUIT_K  'q'

/* CROSS-PLATFORM STUFF */
#   if defined(_WIN64) || defined(_WIN32)
#       include <conio.h>
#       define init_term() ;
#       define reset_term() ;
#   elif defined(__unix__) || defined(__APPLE__)
#       include <stdlib.h>
#       include <unistd.h>
#       include <sys/select.h>
#       include <time.h>
#       include <termios.h>
#       include <string.h>
void init_term(void);
void reset_term(void);
int kbhit(void);
int getch(void);
#   endif
#endif
