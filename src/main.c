#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "./graphics.h"
#include "./input.h"

#if defined(_WIN64) || defined(_WIN32)

    #include <windows.h>
    #define usleep(useconds) do {Sleep((useconds) * 0.001);} while(0)

#elif defined(__unix__) || defined(__APPLE__)

    #include <unistd.h>

#endif

static int main_loop(void);
static int update_direction(snake_t *snake);

int main(void)
{
    int exit;

    exit = init_term();
    if (exit != 0) {
        reset_term(); // try restoring previous state
        return exit;
    }

    exit = main_loop();
    if (exit != 0) {
        reset_term();
        return exit;
    }

    exit = reset_term();
    if (exit != 0) {
        reset_term(); // try again, just to be sure
        return exit;
    }
    return exit;
}

static int main_loop(void)
{
    map_t *main_map, initial_state;
    snake_t *snake;

    bool loop = true;
    int exit;

    main_map = create_map(30, 20);
    snake = create_snake();

    if (!snake || !main_map)
        return 1;

    save_map_state(main_map, &initial_state);
    spawn_food(main_map);
    // Intro
    printf("SNEK TIME!\n\rUse arrow keys or WASD to move. If you want to quit "
           "press 'q'\n\r");
    for (int i = 10; i >= 0; i--) {
        printf("%d\n\r", i);
        usleep(1000000);
    }
    while (loop) {
        clrscr();
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
                usleep(400000 - (get_snake_size(snake) * 1000));
            }
        }
    }
    restore_map_state(main_map, &initial_state); // flush map
    destroy_food(); // cleanup
    destroy_snake(snake);
    destroy_map(main_map);
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
                get_snake_direction(snake) = UP;
                break;
            case DOWN_K:
            case DOWN_ARR_K:
                get_snake_direction(snake) = DOWN;
                break;
            case RIGHT_K:
            case RIGHT_ARR_K:
                get_snake_direction(snake) = RIGHT;
                break;
            case LEFT_K:
            case LEFT_ARR_K:
                get_snake_direction(snake) = LEFT;
                break;
            case QUIT_K:
                return -1;
                break;
            case ESC_K:
                getch(); // [
                return update_direction(snake);
                break;
        }
    }
    return 0;
}
