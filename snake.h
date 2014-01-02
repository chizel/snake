#ifndef SNAKE_H
#define SNAKE_H
#define BOARD_SIZE_X 20
#define BOARD_SIZE_Y 20
#define UP    1
#define LEFT  2
#define DOWN  3
#define RIGHT 4
#define COUNT_OF_OBJECTS 20

typedef struct
{
    uint8_t x;
    uint8_t y;
}snake_item;

typedef struct
{
    snake_item item[255];
    uint8_t size;
    uint8_t head;
    uint8_t where_to_move;
}game_snake;

int init_snake(game_snake *);
int print_snake(game_snake *);
int draw_borders(game_snake *);
int move_snake(game_snake *, int);
int move_up(game_snake *);
int move_down(game_snake *);
int move_left(game_snake *);
int move_right(game_snake *);
int add_item(game_snake *);
int check_snake(game_snake *);

#endif
