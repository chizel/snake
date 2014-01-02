#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <ncurses.h>
#include "snake.h"


uint8_t field[BOARD_SIZE_Y + 1][BOARD_SIZE_X + 1];

int main(int argc, char *argv[])
{
    game_snake snake = {.size = 3};
    uint8_t i;
    wchar_t ch;

    init_snake(&snake);
    initscr();
    keypad(stdscr, TRUE);
    //hide cursor
    curs_set(0);
    move(1,1);
    printw("For starting new game press any key.");
    ch = getch();
    clear();

    generate_field();
    print_snake(&snake);
    draw_borders(&snake);
    refresh();
    //don't wait getch();
    nodelay(stdscr,TRUE);

    while(1)
    {
        ch = getch();

        switch(ch)
        {
            case KEY_UP:
                if(snake.where_to_move != DOWN)
                {
                    move_snake(&snake, UP);
                    print_snake(&snake);
                }
                break;
            case KEY_DOWN:
                if(snake.where_to_move != UP)
                {
                    move_snake(&snake, DOWN);
                    print_snake(&snake);
                }
                break;
            case KEY_LEFT:
                if(snake.where_to_move != RIGHT)
                {
                    move_snake(&snake, LEFT);
                    print_snake(&snake);
                }
                break;
            case KEY_RIGHT:
                if(snake.where_to_move != LEFT)
                {
                    move_snake(&snake, RIGHT);
                    print_snake(&snake);
                }
                break;
            case ' ':
                sleep(10);
                break;
            case 'q':
                endwin();
                exit(0);
                break;
            case 'a':
                add_item(&snake);
            default:
                move_snake(&snake, snake.where_to_move);
                print_snake(&snake);
        }
    }

    endwin();
    return EXIT_SUCCESS;
}
