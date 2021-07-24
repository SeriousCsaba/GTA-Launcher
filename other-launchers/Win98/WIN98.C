#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEMP "WinTemp"
#define ON "0;1;47"
#define OFF "0"

void menu(int selected, int wait)
{
    if (wait && selected > 1)
        system("pause");
    system("cls");
    printf("\n   Microsoft Windows 98 startup Process\n");
    printf("   อออออออออออออออออออออออออออออออออออออ\n\n");
    printf("      [%sm1. Locating hard drives[0m\n", selected == 1 ? ON : OFF);
    printf("      [%sm2. Checking drive letters[0m\n", selected == 2 ? ON : OFF);
    printf("      [%sm3. Preparing hard drives[0m\n", selected == 3 ? ON : OFF);
    printf("      [%sm4. Mounting hard drives[0m\n", selected == 4 ? ON : OFF);
    printf("      [%sm5. Starting Windows[0m\n\n", selected == 5 ? ON : OFF);
}

void checkExist(char *driveletter)
{
    FILE *f;
    char name[8];
    printf("Looking for %s.img...\n", driveletter);
    strcpy(name, driveletter);
    strcat(name, ".img");
    f = fopen(name, "r");
    if (f == NULL)
    {
        printf("Fatal error: Can't open %s.img\n", driveletter);
        exit(0);
    }
    printf("Hard drive found!\n\n");
    fclose(f);
}

int driveExists(char *driveletter)
{
    FILE *f;
    char temp[16];
    strcpy(temp, driveletter);
    strcat(temp, ":\\");
    strcat(temp, TEMP);
    f = fopen(temp, "w");
    fclose(f);
    remove(temp);
    return f != NULL;
}

char *systemReturn(char *command)
{
    FILE *f;
    char str[1024];
    strcpy(str, command);
    strcat(str, " >");
    strcat(str, TEMP);
    system(str);
    f = fopen(TEMP, "r");
    fgets(str, sizeof(str), f);
    fclose(f);
    remove(TEMP);
    return str;
}

int commandExists(char *command)
{
    return systemReturn(command)[0] != 'I';
}

void checkLetter(char *driveletter)
{
    char mountc[1024];
    char mountcdir[1024];
    char unmount[16];
    int i, len;
    if (!driveExists(driveletter))
        printf("Drive %s is free!\n\n", driveletter);
    else
    {
        if (strcmp(driveletter, "C") == 0)
        {
            if (commandExists("remount"))
            {
                printf("Drive %s already exists. ", driveletter);
                system("remount c: x:");
                system("x:");
                printf("Success.\n\n");
                return;
            }
            strcpy(mountc, systemReturn("mount c ."));
            for (i = strlen(mountc); i >= 0; i--)
                if (mountc[i] == ':')
                    break;
            i--;
            if (i == -2)
            {
                for (i = 0; i < strlen(mountc); i++)
                    if (mountc[i] == '\\')
                        break;
                for (; i >= 0; i--)
                    if (mountc[i] == ' ')
                        break;
                i++;
            }
            len = strlen(mountc) - i - 2;
            strncpy(mountcdir, mountc + i, 1024);
            mountcdir[len] = '\0';
            strcpy(mountc, "mount x \"");
            strcat(mountc, mountcdir);
            strcat(mountc, "\" >nul");
            system(mountc);
            system("x:");
        }
        printf("Drive %s already exists. Removing drive...\n", driveletter);
        strcpy(unmount, "mount -u ");
        strcat(unmount, driveletter);
        system(unmount);
        printf("\n");
    }
}

char *checkSize(char *driveletter)
{
    FILE *f;
    char name[8];
    int size;
    strcpy(name, driveletter);
    strcat(name, ".img");
    f = fopen(name, "r");
    fseek(f, 0L, SEEK_END);
    size = ftell(f) / 1048576;
    fclose(f);
    printf("Size of %s is %dmb.\n\n", name, size);
    if (size > 2000)
        return "64,1023";
    if (size > 1000)
        return "64,520";
    if (size > 500)
        return "32,520";
    if (size > 200)
        return "16,520";
    printf("Fatal error: Incorrect hard drive size!\n");
    exit(0);
    return "";
}

void mount(char *driveletter, char *size)
{
    char s[64];
    printf("Mounting %s...\n", driveletter);
    strcpy(s, "imgmount ");
    strcat(s, driveletter);
    strcat(s, ": ");
    strcat(s, driveletter);
    strcat(s, ".img -t hdd -fs fat -size 512,63,");
    strcat(s, size);
    system(s);
    printf("\n");
}

void run(int debug)
{
    printf("Starting Windows...\n");
    system("mount -u X >nul");
    if (debug)
        system("pause");
    system("boot -l c");
}

void main()
{
    char *sizeC;
    char *sizeD;
    FILE *f = fopen("debug", "r");
    int debug = (f == NULL ? 0 : 1);
    fclose(f);

    menu(1, debug);
    checkExist("C");
    checkExist("D");

    menu(2, debug);
    checkLetter("C");
    checkLetter("D");

    menu(3, debug);
    sizeC = checkSize("C");
    sizeD = checkSize("D");

    menu(4, debug);
    mount("C", sizeC);
    mount("D", sizeD);

    menu(5, debug);
    run(debug);
}
