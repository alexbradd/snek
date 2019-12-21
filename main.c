#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "./graphics.h"
#include "./input.h"

#if defined(_WIN64) || defined(_WIN32)

#include <windows.h>
#define usleep(useconds) do {Sleep((useconds) * 1000);} while(0)

#elif defined(__unix__) || defined(__APPLE__)

#include <unistd.h>

#endif

static map_t *main_map;
static snake_t *snake;

static int main_loop(void);
static int update_direction(snake_t *snake);

int main(void)
{
    int exit;

    main_map = create_map(30, 20);
    snake = create_snake();

    if (!snake || !main_map)
        return 1;

    init_term();

    exit = main_loop();

    destroy_snake(snake);
    destroy_map(main_map);

    reset_term();

    return exit;
}

static int main_loop(void)
{
    bool loop = true;
    int exit;
    map_t initial_state;

    save_map_state(main_map, &initial_state);
    spawn_food(main_map);
    while (loop) {
        clrscr();
        printf("SNEK! Info: press 'q' to quit and w-a-s-d to move\n\r");
        restore_map_state(main_map, &initial_state); // flush map
        exit = update_direction(snake);
        if (exit != 0)
            loop = false;
        if (loop) {
            exit = move_snake(snake, main_map);
            if (exit == SNAKE_DED) {
                printf("You died!!\n\r");
                loop = false;
            } else {
                if (exit == SNAKE_LONG)
                    printf("MAX LENGTH!!\n\r");
                apply_snake(main_map, snake); // draw snake
                apply_food(main_map); // draw food
                draw_map(main_map); // draw everything
                usleep(500000 - (get_snake_size(snake) * 500));
            }
        }
    }
    restore_map_state(main_map, &initial_state); // flush map
    destroy_food(); // cleanup
    return 0;
}

int update_direction(snake_t *snake)
{
    int key;
    if (kbhit()) { // get input
        key = getch();
        switch (key) {
            case UP_K:
            case UP_ARR_K:
                change_direction(snake, UP);
                break;
            case DOWN_K:
            case DOWN_ARR_K:
                change_direction(snake, DOWN);
                break;
            case RIGHT_K:
            case RIGHT_ARR_K:
                change_direction(snake, RIGHT);
                break;
            case LEFT_K:
            case LEFT_ARR_K:
                change_direction(snake, LEFT);
                break;
            case QUIT_K:
                return -1;
                break;
            case ESC_K:
                getch(); // [
                update_direction(snake);
                break;
        }
    }
    return 0;
}
