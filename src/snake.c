#include <stdio.h>
#include <windows.h>

#define SNAKE_HEAD_UP '^'
#define SNAKE_HEAD_DOWN 'v'
#define SNAKE_HEAD_LEFT '<'
#define SNAKE_HEAD_RIGHT '>'

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define SNAKE_BODY '@'
#define WALL '#'
#define FOOD 'o'
#define AIR ' '

#define MAP_WIDTH 40
#define MAP_HEIGHT 40
#define SNAKE_ARRAY_SIZE (MAP_WIDTH * MAP_HEIGHT)

#define HEAD_INDEX 0
#define START_BODY_INDEX 1

#define NEW_LINE() putchar('\n')

HANDLE hConsole;

typedef struct pos {
    int x;
    int y;
} pos_t;

pos_t snake[SNAKE_ARRAY_SIZE];
int snake_length;
int tail_index;
int move_direction;

void setCursor(int x, int y);

void drawSnake();
void drawMap();

void printChar(char ch);
void repeatChar(char ch, int count);
void putCursorOnMap(pos_t pos);

int main(void) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    drawMap();

    /* Set cursor at the end of the output */
    setCursor(0, MAP_HEIGHT + 2);

    return 0;
}

void drawMap() {
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

void drawSnake() {
    int i;
    putCursorOnMap(snake[HEAD_INDEX]);
    printChar('&');
    for (i = START_BODY_INDEX; i < snake_length; ++i) {
        putCursorOnMap(snake[i]);
        printChar(SNAKE_BODY);
    }
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

void putCursorOnMap(pos_t pos) {
    setCursor((pos.x + 1) * 2, pos.y + 1);
}
