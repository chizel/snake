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
    snake->size = 4;
    snake->head = snake->size - 1;
    snake->where_to_move = RIGHT;
    snake->item[0].x = 1;
    snake->item[0].y = 1;
    snake->item[1].x = 2;
    snake->item[1].y = 1;
    snake->item[2].x = 3;
    snake->item[2].y = 1;
    snake->item[3].x = 4;
    snake->item[3].y = 1;
    return 0;
}

int draw_borders(game_snake *snake)
{
    uint8_t i;

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

    return 0;
}

int move_snake(game_snake *snake, int where)
{
    //moves: 1-up, 2-left, 3-down, 4-right
    uint8_t i;

    switch(where)
    {
        case UP:
            if( (snake->item[0].x == snake->item[1].x) && (snake->where_to_move == DOWN) )
            {
                //head and second element of snake are in the same line
                //move snake to the other side
                reverse_snake(snake);
                move_up(snake);
            }
            else
            {
                move_up(snake);
            }
            break;
        case LEFT:
            if( (snake->item[0].y == snake->item[1].y) && (snake->where_to_move == RIGHT) )
            {
                reverse_snake(snake);
                move_left(snake);
            }
            else
            {
                move_left(snake);
            }
            break;
        case DOWN:
            if( (snake->item[0].x == snake->item[1].x) && (snake->where_to_move == UP) )
            {
                reverse_snake(snake);
                move_down(snake);
            }
            else
            {
                move_down(snake);
            }
            break;
        case RIGHT:
            if( (snake->item[snake->head].y == snake->item[snake->head - 1].y) && (snake->where_to_move == LEFT) )
            {
                reverse_snake(snake);
                move_right(snake);
            }
            else
            {
                move_right(snake);
            }
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

int reverse_snake(game_snake *snake)
{
    snake_item tmp;
    uint8_t i;

    for(i = 0; i < snake->size / 2; i++)
    {
        tmp = snake->item[i];
        snake->item[i] = snake->item[snake->head - i];
        snake->item[snake->head - i] = tmp;
    }

    return 0;
}    

int check_snake(game_snake *snake)
{
    //is snake try to go to filled element
    uint8_t i;

    for( i = 0; i < snake->head; i++)
    {
        if( (snake->item[i].x == snake->item[snake->head].x) && (snake->item[i].y == snake->item[snake->head].y) )
        {
            move(3,3);
            printw("YOU LOSE!");
            refresh();
            sleep(4);
            endwin();
            exit(0);
        }
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

int show_elements(game_snake *snake)
{
    uint8_t i;

    for(i = 0; i <= snake->head ; i++)
    {
        move(snake->item[i].y, snake->item[i].x);
        addch('*');
        move(i + 5, 0);
        printw("i: %d; y: %d, x: %d", i, snake->item[i].y, snake->item[i].x);
    }

    refresh();
    sleep(4);
    return 0;
}

/*int generate_field()*/
/*{*/
    /*field[0][0];*/
/*}*/
