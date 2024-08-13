#include <menu.h>
#include <sound16.h>

#define GTA 1
#define GTAUK 2
#define GTA61 3

#define SETTINGS 0
#define LOW 1
#define HIGH 2
#define _3D 3

#define SOUNDFILE "gtadata\\audio\\level000.raw"

void cleanup()
{
    system("imgmount -u d");
    system("cd gtados");
    system("del dino.bat");
    system("del DUMP*.TGA");
    system("del *.SWP");
    system("cd ..");
}

int selectSound()
{
    int key = select();
    switch (key)
    {
    case UP:
        soundPlay(SOUNDFILE, MONO, 38000L, 59720L, 701434L);
        break;
    case DOWN:
        soundPlay(SOUNDFILE, MONO, 38000L, 62222L, 761154L);
        break;
    case LEFT:
        soundPlay(SOUNDFILE, MONO, 44100L, 24448L, 475440L);
        break;
    case RIGHT:
        soundPlay(SOUNDFILE, MONO, 44100L, 24448L, 475440L);
        break;
    case ENTER:
        soundPlay(SOUNDFILE, STEREO, 24000L, 135136L, 340304L);
        break;
    case ESC:
        soundPlay(SOUNDFILE, MONO, 22050L, 36300L, 499888L);
        break;
    }
    return key;
}

void mountGame(int game)
{
    if (game == GTA)
        system("imgmount d GTA.cue -t iso -fs iso");
    else
        system("imgmount d GTAUK.cue -t iso -fs iso");
}

void run(int game, int mode)
{
    char gta[16];

    mountGame(game);
    system("cd gtados");
    system("cls");

    if (mode == SETTINGS)
        strcpy(gta, "k");
    else
    {
        strcpy(gta, "gta");
        switch (mode)
        {
        case LOW:
            strcat(gta, "8");
            break;
        case _3D:
            strcat(gta, "fx");
            break;
        default:
            strcat(gta, "24");
            break;
        }
    }
    switch (game)
    {
    case GTAUK:
        strcat(gta, "_uk");
        break;
    case GTA61:
        strcat(gta, "_61");
        break;
    }
    strcat(gta, ".exe");
    system(gta);
    if (mode == SETTINGS)
        system("dino.bat");
    cleanup();
}

void mainMenu()
{
    int game = GTA;
    int mode = HIGH;
    while (1)
    {
        system("cls");
        system("type gtamenu\\LOGO.ans");
        switch (game)
        {
        case GTA:
            system("type gtamenu\\GTA.ans");
            break;
        case GTAUK:
            system("type gtamenu\\GTAUK.ans");
            break;
        case GTA61:
            system("type gtamenu\\GTA61.ans");
            break;
        }

        gotoxy(52, 24);
        switch (mode)
        {
        case LOW:
            printf("[1;33mLOW  COLOR MODE");
            break;
        case HIGH:
            printf("[1;33mHIGH COLOR MODE");
            break;
        case _3D:
            printf("[1;33m   3DFX MODE   ");
            break;
        case SETTINGS:
            printf("[1;33m    SETTINGS   ");
            break;
        }

        switch (selectSound())
        {
        case ESC:
            return;
        case ENTER:
            run(game, mode);
            break;
        case RIGHT:
            if (mode < _3D)
                mode++;
            else
                mode = SETTINGS;
            if (game == GTA61 && mode == LOW)
                mode = HIGH;
            break;
        case LEFT:
            if (mode > SETTINGS)
                mode--;
            else
                mode = _3D;
            if (game == GTA61 && mode == LOW)
                mode = SETTINGS;
            break;
        case UP:
            if (game > GTA)
                game--;
            mode = HIGH;
            break;
        case DOWN:
            if (game < GTA61)
                game++;
            mode = HIGH;
            break;
        }
    }
}

void main()
{
    SHOW_CONTROLS = 0;
    cleanup();
    soundInit();
    mainMenu();
    soundDeinit();
    printf("[0m");
    system("cls");
}