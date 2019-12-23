#ifndef __INPUT_H__ // __INPUT_H__

#define __INPUT_H__ // Guard macro

#define   UP_K          'w'
#define   UP_ARR_K      'A'

#define   DOWN_K        's'
#define   DOWN_ARR_K    'B'

#define   RIGHT_K       'd'
#define   RIGHT_ARR_K   'C'

#define   LEFT_K        'a'
#define   LEFT_ARR_K    'D'

#define   QUIT_K        'q'
#define   ESC_K         '\x1b'

#if defined(_WIN64) || defined(_WIN32) // OS

// #include <conio.h>

int init_term(void);
int reset_term(void);

#define kbhit _kbhit
// #define getch _getch

/*int kbhit(void);*/
int getch(void);

#elif defined(__unix__) || defined(__APPLE__)

int init_term(void);
int reset_term(void);
int kbhit(void);
int getch(void);

#endif // OS

#endif // __INPUT_H__
