#include "./graphics.h"

static food_t *current_food = NULL;

map_t *create_map(int x_tiles, int y_tiles)
{
    map_t *map = malloc(sizeof(map_t));
    if (map) {
        map->x_size = x_tiles;
        map->y_size = y_tiles;
        for (int y = 0; y < MAX_Y; y++) {
            for (int x = 0; x < MAX_X; x++) {
                if (x < x_tiles || y < y_tiles)
                    map_at(map, x, y) = EMPTY;
                else
                    map_at(map, x, y) = INVALID;
            }
        }
        for (int y = 0; y < map_y_size(map); y++) {
            map_at(map, 0, y) = WALL;
            map_at(map, map_x_size(map) - 1, y) = WALL;
        }
        for (int x = 0; x < map_x_size(map); x++) {
            map_at(map, x, 0) = WALL;
            map_at(map, x, map_y_size(map) - 1) = WALL;
        }
    }
    return map;
}

void destroy_map(map_t *map)
{
    if (map) {
        free(map);
        map = NULL;
    }
}

void draw_map(const map_t *map)
{
    for (int y = 0; y < map_y_size(map); y++) {
        for (int x = 0; x < map_x_size(map); x++) {
            switch (map_at(map, x, y)) {
                case EMPTY:
                    fprintf(stdout, "%c", EMPTY_C);
                    break;
                case WALL:
                    fprintf(stdout, "%c", WALL_C);
                    break;
                case HEAD:
                    fprintf(stdout, "%c", HEAD_C);
                    break;
                case BODY:
                    fprintf(stdout, "%c", BODY_C);
                    break;
                case FOOD:
                    fprintf(stdout, "%c", FOOD_C);
                    break;
                case INVALID:
                default:
                    break; // fall through
            }
        }
        printf("\n\r");
    }
}

void save_map_state(const map_t *map, map_t *prev_state)
{
    if (map && prev_state)
        memcpy(prev_state, map, sizeof(map_t));
}

void restore_map_state(map_t *map, const map_t *prev_state)
{
    if (map && prev_state)
        memcpy(map, prev_state, sizeof(map_t));
}

snake_t *create_snake(void)
{
    snake_t *s = malloc(sizeof(snake_t));
    if (s) {
        get_snake_size(s) = 2;
        get_snake_x(s, 0) = SPAWN_X;
        get_snake_y(s, 0) = SPAWN_Y;
        get_snake_x(s, 1) = SPAWN_X + 1;
        get_snake_y(s, 1) = SPAWN_Y;
        change_direction(s, UP);
    }
    return s;
}

void destroy_snake(snake_t *snake)
{
    if (snake) {
        free(snake);
        snake = NULL;
    }
}

int move_snake(snake_t *snake, map_t *map)
{
    snake_segment_t temp1, temp2;

    temp1 = get_snake_seg(snake)[0];
    switch (get_direction(snake)) {
        case UP:
            get_snake_y(snake, 0)--;
            break;
        case DOWN:
            get_snake_y(snake, 0)++;
            break;
        case LEFT:
            get_snake_x(snake, 0)--;
            break;
        case RIGHT:
            get_snake_x(snake, 0)++;
            break;
    }
    for (int i = 1; i < get_snake_size(snake); i++) {
        // move segments
        if (i % 2 == 0) {
            temp1 = get_snake_seg(snake)[i];
            get_snake_seg(snake)[i] = temp2;
        } else {
            temp2 = get_snake_seg(snake)[i];
            get_snake_seg(snake)[i] = temp1;
        }
    }
    // WALL COLLISION
    if (map_at(map, get_snake_x(snake, 0), get_snake_y(snake, 0)) == WALL)
        return SNAKE_DED;
    // AUTO COLLISION
    for (int i = 0; i < get_snake_size(snake); i++) {
        for (int j = i + 1; j < get_snake_size(snake); j++) {
            if (get_snake_x(snake, i) == get_snake_x(snake, j) &&
                    get_snake_y(snake, i) == get_snake_y(snake, j))
                return SNAKE_DED;
        }
    }
    // FOOD
    if (get_snake_x(snake, 0) == get_food_x(current_food) &&
            get_snake_y(snake, 0) == get_food_y(current_food)) {
        spawn_food(map);
        if (get_snake_size(snake) >= MAX_LEN)
            return SNAKE_LONG;
        get_snake_size(snake)++;
        /* if going up, add one down etc */
        switch (get_direction(snake)) {
            case UP:
                get_snake_x(snake, get_snake_size(snake) - 1) =
                    get_snake_x(snake, get_snake_size(snake) - 2);
                get_snake_y(snake, get_snake_size(snake) - 1) =
                    get_snake_y(snake, get_snake_size(snake) - 2) + 1;
                break;
            case DOWN:
                get_snake_x(snake, get_snake_size(snake) - 1) =
                    get_snake_x(snake, get_snake_size(snake) - 2);
                get_snake_y(snake, get_snake_size(snake) - 1) =
                    get_snake_y(snake, get_snake_size(snake) - 2) - 1;
                break;
            case LEFT:
                get_snake_x(snake, get_snake_size(snake) - 1) =
                    get_snake_x(snake, get_snake_size(snake) - 2) + 1;
                get_snake_y(snake, get_snake_size(snake) - 1) =
                    get_snake_y(snake, get_snake_size(snake) - 2);
                break;
            case RIGHT:
                get_snake_x(snake, get_snake_size(snake) - 1) =
                    get_snake_x(snake, get_snake_size(snake) - 2) - 1;
                get_snake_y(snake, get_snake_size(snake) - 1) =
                    get_snake_y(snake, get_snake_size(snake) - 2);
                break;
        }
    }
    return 0;
}

void apply_snake(map_t *map, const snake_t *snake)
{
    map_at(map, get_snake_x(snake, 0), get_snake_y(snake, 0)) = HEAD;
    for (int i = 1; i < get_snake_size(snake); i++)
        map_at(map, get_snake_x(snake, i), get_snake_y(snake, i)) = BODY;
}

void apply_food(map_t *map)
{
    if (current_food)
        map_at(map, get_food_x(current_food), get_food_y(current_food)) = FOOD;
}

void spawn_food(map_t *map)
{
    food_t *f = malloc(sizeof(food_t));
    int x, y;
    if (f) {
        srand(time(NULL));
        x = rand() % map_x_size(map);
        y = rand() % map_y_size(map);
        if (current_food) {
            while ((x == get_food_x(current_food) && y == get_food_y(current_food)) ||
                    map_at(map, x, y) == WALL) {
                x = rand() % map_x_size(map);
                y = rand() % map_y_size(map);
            }
        } else {
            while (map_at(map, x, y) == WALL) {
                x = rand() % map_x_size(map);
                y = rand() % map_y_size(map);
            }

        }
        get_food_x(f) = x;
        get_food_y(f) = y;
        destroy_food();
        current_food = f;
    }
}

void destroy_food(void)
{
    if (current_food) {
        free(current_food);
        current_food = NULL;
    }
}
