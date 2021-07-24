#include <stdlib.h>
#include <string.h>
#include <menu.h>

#define MAXSIZE 64
#define MNU_DOOM1 1
#define MNU_DOOM2 2
#define MNU_QUIT 3

#define ON "[0m#[31m"
#define OFF "[0m [31m"

void main()
{
    int selected = MNU_DOOM1;
    char games[][MAXSIZE] =
        {"DOOM",
         "DOOM2",
         "QUIT"};
    char str[MAXSIZE];
    FOUR_DIRECTIONS = 0;
    while (selected)
    {
        system("cls");
        strcpy(str, "type ");
        strcat(str, games[selected - 1]);
        strcat(str, ".ANS");
        system(str);
        printf("%s Play Doom\n", selected == MNU_DOOM1 ? ON : OFF);
        printf("%s Play Doom II: Hell on Earth\n", selected == MNU_DOOM2 ? ON : OFF);
        printf("%s Quit\n\n", selected == MNU_QUIT ? ON : OFF);
        switch (select())
        {
        case ESC:
            selected = 0;
        case ENTER:
            switch (selected)
            {
            case MNU_DOOM1:
            case MNU_DOOM2:
                system("cls");
                strcpy(str, "cd ");
                strcat(str, games[selected - 1]);
                system(str);
                strcpy(str, games[selected - 1]);
                strcat(str, ".exe");
                system(str);
                system("cd ..");
                break;
            case MNU_QUIT:
                selected = 0;
                break;
            }
            break;
        case UP:
            if (selected > MNU_DOOM1)
                selected--;
            break;
        case DOWN:
            if (selected < MNU_QUIT)
                selected++;
            break;
        }
    }
    printf("[0m");
    system("cls");
}