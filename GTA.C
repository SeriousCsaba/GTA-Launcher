#include <menu.h>
#include <sound.h>

#define START 1
#define SETTINGS 2

#define GTA 1
#define GTAUK 2
#define GTA61 3

#define DEFAULT 0
#define LOW 1
#define HIGH 2
#define _3D 3

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
        sb_single_play("gtamenu\\CHANGE.raw");
        break;
    case DOWN:
        sb_single_play("gtamenu\\CHANGE.raw");
        break;
    case LEFT:
        sb_single_play("gtamenu\\DOWN.raw");
        break;
    case RIGHT:
        sb_single_play("gtamenu\\UP.raw");
        break;
    case ENTER:
        sb_single_play("gtamenu\\ENTER.raw");
        break;
    case ESC:
        sb_single_play("gtamenu\\BACK.raw");
        break;
    }
    return key;
}

void mountMusic(int game)
{
    if (game == GTA)
        system("imgmount d gtamusic\\music.cue -t iso -fs iso");
    else
        system("imgmount d gtamusic\\music_uk.cue -t iso -fs iso");
}

void runSettings(int game)
{
    char gta[16];

    mountMusic(game);
    system("cd gtados");
    system("cls");

    strcpy(gta, "k");
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
    system("dino.bat");
    cleanup();
}

void run(int game, int gfx)
{
    char gta[16];

    mountMusic(game);
    system("cd gtados");
    system("cls");

    strcpy(gta, "gta");
    switch (gfx)
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
    cleanup();
}

void settingsMenu()
{
    int game = GTA;
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
        gotoxy(15 + (game - 1) * 19, 24);
        printf("[0;1mSETTINGS");

        switch (selectSound())
        {
        case ESC:
            return;
        case ENTER:
            runSettings(game);
            break;
        case RIGHT:
            if (game < GTA61)
                game++;
            break;
        case LEFT:
            if (game > GTA)
                game--;
            break;
        }
    }
}

void playMenu()
{
    int game = GTA;
    int gfx = DEFAULT;
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
        gotoxy(15 + (game - 1) * 19, 24);
        printf("[0;1mPLAY");

        if (gfx != DEFAULT)
        {
            gotoxy(17 + (game - 1) * 19 + (gfx != HIGH ? gfx : 0), 25);
            switch (gfx)
            {
            case LOW:
                printf("[33mLOW COLOR");
                break;
            case HIGH:
                printf("[33mHIGH  COLOR");
                break;
            case _3D:
                printf("[33m3D FX");
                break;
            }
        }

        switch (selectSound())
        {
        case ESC:
            return;
        case ENTER:
            run(game, gfx);
            break;
        case RIGHT:
            if (game < GTA61)
                game++;
            if (game == GTA61 && gfx == LOW)
                gfx = DEFAULT;
            break;
        case LEFT:
            if (game > GTA)
                game--;
            break;
        case UP:
            if (gfx < _3D)
                gfx++;
            else
                gfx = DEFAULT;
            if (game == GTA61 && gfx == LOW)
                gfx = HIGH;
            break;
        case DOWN:
            if (gfx > DEFAULT)
                gfx--;
            else
                gfx = _3D;
            if (game == GTA61 && gfx == LOW)
                gfx = DEFAULT;
            break;
        }
    }
}

void mainMenu()
{
    int selected = START;
    while (1)
    {
        system("cls");
        system("type gtamenu\\LOGO.ans");
        switch (selected)
        {
        case START:
            system("type gtamenu\\START.ans");
            break;
        case SETTINGS:
            system("type gtamenu\\SETTINGS.ans");
            break;
        }

        switch (selectSound())
        {
        case ESC:
            return;
        case ENTER:
            switch (selected)
            {
            case START:
                playMenu();
                break;
            case SETTINGS:
                settingsMenu();
                break;
            }
            break;
        case RIGHT:
            selected = SETTINGS;
            break;
        case LEFT:
            selected = START;
            break;
        }
    }
}

void main()
{
    SHOW_CONTROLS = 0;
    cleanup();
    sb_detect();
    sb_init();
    mainMenu();
    sb_deinit();
    printf("[0m");
    system("cls");
}