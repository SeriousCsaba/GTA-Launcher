#include <stdlib.h>
#include <string.h>
#include <menu.h>

#define MAXSIZE 64
#define MNU_DESC1 1
#define MNU_DESC2 2
#define MNU_QUIT 3

#define ON "[0m#[31m"
#define OFF "[0m [31m"

void main()
{
    char games[][MAXSIZE] =
        {"DESCENT",
         "DESCENT2",
         "QUIT"};
    int selected = MNU_DESC1;
    char str[MAXSIZE];
    FOUR_DIRECTIONS = 0;
    while (selected)
    {
        system("cls");
        strcpy(str, "type ");
        strcat(str, games[selected - 1]);
        strcat(str, ".ANS");
        system(str);
        printf("%s Play Descent\n", selected == MNU_DESC1 ? ON : OFF);
        printf("%s Play Descent 2\n", selected == MNU_DESC2 ? ON : OFF);
        printf("%s Quit\n\n", selected == MNU_QUIT ? ON : OFF);
        switch (select())
        {
        case ESC:
            selected = 0;
        case ENTER:
            switch (selected)
            {
            case MNU_DESC1:
            case MNU_DESC2:
                system("cls");
                strcpy(str, "cd ");
                strcat(str, games[selected - 1]);
                system(str);
                system("del *.swp");
                strcpy(str, games[selected - 1]);
                strcat(str, ".exe");
                system(str);
                system("del *.swp");
                system("cd ..");
                break;
            case MNU_QUIT:
                selected = 0;
                break;
            }
            break;
        case UP:
            if (selected > MNU_DESC1)
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