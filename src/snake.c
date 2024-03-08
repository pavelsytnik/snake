#include <stdio.h>
#include <windows.h>

/* Snake head according to its direction */
#define UP_DIR '^'
#define DOWN_DIR 'v'
#define LEFT_DIR '<'
#define RIGHT_DIR '>'

#define SNAKE_BODY '@'
#define WALL '#'
#define FOOD 'o'
#define AIR ' '

#define MAP_WIDTH 40
#define MAP_HEIGHT 40
#define SNAKE_ARRAY_SIZE (MAP_WIDTH * MAP_HEIGHT)

#define NEW_LINE() putchar('\n')

HANDLE hConsole;
void setCursor(int x, int y);

void draw();
void printChar(char ch);
void repeatChar(char ch, int count);
void putCursorOnMap(int x, int y);

int main(void) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    draw();
    return 0;
}

void draw() {
    int i, j;

    repeatChar(WALL, MAP_WIDTH + 2);
    NEW_LINE();

    for (i = 0; i < MAP_HEIGHT; ++i) {
        printChar(WALL);
        repeatChar(AIR, MAP_WIDTH);
        printChar(WALL);
        NEW_LINE();
    }

    repeatChar(WALL, MAP_WIDTH + 2);
    NEW_LINE();
}

void printChar(char ch) {
    putchar(ch);
    putchar(' ');
}

void repeatChar(char ch, int count) {
    int i;
    for (i = 0; i < count; ++i) {
        printChar(ch);
    }
}

void setCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(hConsole, coord);
}

void putCursorOnMap(int x, int y) {
    setCursor((x + 1) * 2, y + 1);
}
