#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <unistd.h>
#include <termios.h>

char* str_concat(char* str1, char* str2)
{
    char* str3 = (char*)malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(str3, str1);
    strcat(str3, str2);
    return str3;
}

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

int itoa(int n, char* str)
{
    return asprintf(&str, "%d", n);
}

bool srand_init = false;

int random_int(int max)
{
    if(!srand_init)
    {
        srand(time(NULL));
    }

    return rand() % (max+1);
}

float lerp (float a, float b, float f) {
    return a + f * (b - a);
}