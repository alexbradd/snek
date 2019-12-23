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

int init_term(void);
int reset_term(void);
int kbhit(void);
int getch(void);

#endif // __INPUT_H__
