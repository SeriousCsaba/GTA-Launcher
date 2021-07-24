#include <stdio.h>
#include <conio.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define ESC 27

int SHOW_CONTROLS = 1;
int FOUR_DIRECTIONS = 1;
int SKIP_POSITION = 1;

int select()
{
    int key = 0;
    if (SHOW_CONTROLS)
        printf(FOUR_DIRECTIONS ? "    Enter" : "  Enter");
    while (1)
    {
        gotoxy(1, SKIP_POSITION);
        printf("[0;1;0;30m ");
        gotoxy(1, SKIP_POSITION);
        key = getch();
        if (key && key == 224)
            key = getch();
        switch (key)
        {
        case UP:
        case DOWN:
        case ENTER:
        case ESC:
            return key;
        case LEFT:
        case RIGHT:
            if (FOUR_DIRECTIONS)
                return key;
        }
    }
}