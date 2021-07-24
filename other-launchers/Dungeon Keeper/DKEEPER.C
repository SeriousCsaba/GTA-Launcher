#include <stdlib.h>
#include <menu.h>

#define MNU_KEEP 1
#define MNU_DEEP 2
#define MNU_QUIT 3

#define ON 7
#define OFF 0

void main()
{
    int selected = MNU_KEEP;
    FOUR_DIRECTIONS = 0;
    system("imgmount d KDDISK.cue -t iso -fs iso");
    while (selected)
    {
        system("cls");
        system("type DK.ANS");
        printf("\n                   [%dm             DUNGEON  KEEPER             [0m\n", selected == MNU_KEEP ? ON : OFF);
        printf("                   [%dm             DEEPER DUNGEONS             [0m\n", selected == MNU_DEEP ? ON : OFF);
        printf("                   [%dm                   QUIT                  [0m\n\n", selected == MNU_QUIT ? ON : OFF);
        switch (select())
        {
        case ESC:
            selected = 0;
        case ENTER:
            switch (selected)
            {
            case MNU_KEEP:
                system("cls");
                system("KEEPER.exe");
                break;
            case MNU_DEEP:
                system("cls");
                system("DEEPER.exe");
                break;
            case MNU_QUIT:
                selected = 0;
                break;
            }
            break;
        case UP:
            if (selected > MNU_KEEP)
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