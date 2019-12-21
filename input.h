#ifndef __INPUT_H__ // __INPUT_H__

#define __INPUT_H__ // Guard macro

#define UP_K    'w'
#define DOWN_K  's'
#define RIGHT_K 'd'
#define LEFT_K  'a'
#define QUIT_K  'q'

#if defined(_WIN64) || defined(_WIN32) // OS

#include <conio.h>

#define init_term(orig_term_data) ;
#define reset_term(orig_term_data) ;
/*int kbhit(void);*/
/*int getch(void);*/

#elif defined(__unix__) || defined(__APPLE__)

void init_term(void);
void reset_term(void);
int kbhit(void);
int getch(void);

#endif // OS

#endif // __INPUT_H__
