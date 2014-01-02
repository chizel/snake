#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <ncurses.h>
#include "snake.h"

extern uint8_t field[BOARD_SIZE_Y + 1][BOARD_SIZE_X + 1];

int init_snake(game_snake *snake)
{
    snake->size = 3;
    snake->head = snake->size - 1;
    snake->where_to_move = RIGHT;
    snake->item[0].x = 1;
    snake->item[0].y = 1;
    snake->item[1].x = 2;
    snake->item[1].y = 1;
    snake->item[2].x = 3;
    snake->item[2].y = 1;
    return 0;
}

int draw_borders(game_snake *snake)
{
    uint8_t i, j;

    for(i = 0; i <= BOARD_SIZE_X + 1; i++)
    {
        move(0, i);
        addch(ACS_CKBOARD);
        move(BOARD_SIZE_Y + 1, i);
        addch(ACS_CKBOARD);
    }

    for(i = 0; i <= BOARD_SIZE_Y + 1; i++)
    {
        move(i, 0);
        addch(ACS_CKBOARD);
        move(i, BOARD_SIZE_X + 1);
        addch(ACS_CKBOARD);
    }

    for(i = 1; i < BOARD_SIZE_Y; i++)
    {
        for(j = 1; j < BOARD_SIZE_Y; j++)
        {
            if(field[i][j])
            {
                move(i,j);
                addch(ACS_BLOCK);
            }
        }
    }

    return 0;
}

int move_snake(game_snake *snake, int where)
{
    //moves: 1-up, 2-left, 3-down, 4-right
    uint8_t i;

    switch(where)
    {
        case UP:
            move_up(snake);
            break;
        case LEFT:
            move_left(snake);
            break;
        case DOWN:
            move_down(snake);
            break;
        case RIGHT:
            move_right(snake);
            break;
        default:
            ;
            /*exit(0);*/
    }

    check_snake(snake);
    return 0;
}

int move_up(game_snake *snake)
{
    uint8_t i;

    for(i = 0; i < snake->head; i++)
    {
        snake->item[i] = snake->item[i + 1];
    }

    if(snake->item[snake->head].y == 1)
    {
        snake->item[snake->head].y = BOARD_SIZE_Y;
    }
    else
    {
        snake->item[snake->head].y -= 1;
    }

    snake->where_to_move = UP;
    return 0;
}

int move_down(game_snake *snake)
{
    uint8_t i;

    for(i = 0; i < snake->head; i++)
    {
        snake->item[i] = snake->item[i + 1];
    }

    if(snake->item[snake->head].y == BOARD_SIZE_Y)
    {
        snake->item[snake->head].y = 1;
    }
    else
    {
        snake->item[snake->head].y += 1;
    }

    snake->where_to_move = DOWN;
    return 0;
}

int move_left(game_snake *snake)
{
    uint8_t i;

    for(i = 0; i < snake->head; i++)
    {
        snake->item[i] = snake->item[i + 1];
    }

    if(snake->item[snake->head].x == 1)
    {
        snake->item[snake->head].x = BOARD_SIZE_X;
    }
    else
    {
        snake->item[snake->head].x -= 1;
    }

    snake->where_to_move = LEFT;
    return 0;
}

int move_right(game_snake *snake)
{
    uint8_t i;

    for(i = 0; i < snake->head; i++)
    {
        snake->item[i] = snake->item[i + 1];
    }

    if(snake->item[snake->head].x == BOARD_SIZE_X)
    {
        snake->item[snake->head].x = 1;
    }
    else
    {
        snake->item[snake->head].x += 1;
    }

    snake->where_to_move = RIGHT;
    return 0;
}

int print_snake(game_snake *snake)
{
    uint8_t i;

    erase();

    for(i = 0; i < snake->size; i++)
    {
        //row - y, col - x
        move(snake->item[i].y, snake->item[i].x);
        addch(ACS_DIAMOND);
    }

    draw_borders(snake);
    refresh();
    usleep(500000);
    return 0;
}

int check_snake(game_snake *snake)
{
    //is snake try to go to filled element
    uint8_t i, x, y;

    for( i = 0; i < snake->head; i++)
    {
        if( (snake->item[i].x == snake->item[snake->head].x)
                && (snake->item[i].y == snake->item[snake->head].y) )
        {
            game_over();
        }
    }

    x = snake->item[snake->head].x;
    y = snake->item[snake->head].y;

    if(field[y][x])
    {
        game_over();
    }

    return 0;
}

int add_item(game_snake *snake)
{
    snake->item[snake->size].x = snake->item[snake->head].x;
    snake->item[snake->size].y = snake->item[snake->head].y;
    snake->head = snake->size;
    snake->size++;

    switch(snake->where_to_move)
    {
        case UP:
            if(snake->item[snake->head].y == 1)
            {
                snake->item[snake->head].y = BOARD_SIZE_Y;
            }
            else
            {
                snake->item[snake->head].y--;
            }
            break;
        case RIGHT:
            if(snake->item[snake->head].x == BOARD_SIZE_X)
            {
                snake->item[snake->head].x = 1;
            }
            else
            {
                snake->item[snake->head].x++;
            }
            break;
        case DOWN:
            if(snake->item[snake->head].y == BOARD_SIZE_Y)
            {
                snake->item[snake->head].y = 1;
            }
            else
            {
                snake->item[snake->head].y++;
            }
            break;
        case LEFT:
            if(snake->item[snake->head].x == 1)
            {
                snake->item[snake->head].x = BOARD_SIZE_X;
            }
            else
            {
                snake->item[snake->head].x++;
            }
            break;
    };

    print_snake(snake);
    return 0;
}

int generate_field()
{
    uint8_t i, j, y, x;
    //field 1 to BOARD_SIZE_Y, 1 to BOARD_SIZE_X
    srand(time(NULL));

    for(i = 0; i <= BOARD_SIZE_Y; i++)
    {
        for(j = 0; j <= BOARD_SIZE_X; j++)
        {
            field[i][j] = 0;
        }
    }

    for(i = 0; i < COUNT_OF_OBJECTS; i++)
    {
        x = rand() % BOARD_SIZE_X + 1;
        y = rand() % BOARD_SIZE_Y + 1;
        field[y][x] = 1;
    }

    for(x = 1; x <= 4; x++)
    {
        field[1][x] = 0;
    }

    return 0;
}

int game_over()
{
    move(3,3);
    printw("YOU LOSE!");
    refresh();
    sleep(4);
    endwin();
    exit(0);
}
