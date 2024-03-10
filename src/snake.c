#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

SHORT downPressed;
SHORT upPressed;
SHORT leftPressed;
SHORT rightPressed;

typedef struct pos {
    int x;
    int y;
} pos_t;

pos_t snake[SNAKE_ARRAY_SIZE];
int snake_length;
int tail_index;
int move_direction;

pos_t food;

void setCursor(int x, int y);
void hideCursor();

void init();
void input();
void gameLoop();

void drawSnake();
void drawHead();
void drawMap();
void drawFood();

void printChar(char ch);
void repeatChar(char ch, int count);
void putCursorOnMap(pos_t pos);

void generateFood();

int main(void) {

    init();
    drawMap();
    drawSnake();
    drawFood();

    gameLoop();

    /* Set cursor at the end of the output */
    setCursor(0, MAP_HEIGHT + 2);

    return 0;
}

void init() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hideCursor();

    move_direction = RIGHT;
    snake_length = 1;
    snake[HEAD_INDEX].x = MAP_WIDTH / 2;
    snake[HEAD_INDEX].y = MAP_HEIGHT / 2;

    srand(time(NULL));
    generateFood();
}

void gameLoop() {
    int gameOver = 0;
    while (!gameOver) {
        input();
        drawSnake();
        Sleep(150);
    }
}

void input() {
    downPressed = GetAsyncKeyState(VK_DOWN);
    upPressed = GetAsyncKeyState(VK_UP);
    leftPressed = GetAsyncKeyState(VK_LEFT);
    rightPressed = GetAsyncKeyState(VK_RIGHT);

    switch (move_direction) {
        case UP:
        case DOWN:
            if (leftPressed)
                move_direction = LEFT;
            else if (rightPressed)
                move_direction = RIGHT;
            break;
        case LEFT:
        case RIGHT:
            if (upPressed)
                move_direction = UP;
            else if (downPressed)
                move_direction = DOWN;
            break;
    }
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

    drawHead();
    for (i = START_BODY_INDEX; i < snake_length; ++i) {
        putCursorOnMap(snake[i]);
        printChar(SNAKE_BODY);
    }
}

void drawHead() {
    putCursorOnMap(snake[HEAD_INDEX]);
    switch (move_direction) {
        case UP:
            printChar(SNAKE_HEAD_UP);
            break;
        case RIGHT:
            printChar(SNAKE_HEAD_RIGHT);
            break;
        case DOWN:
            printChar(SNAKE_HEAD_DOWN);
            break;
        case LEFT:
            printChar(SNAKE_HEAD_LEFT);
            break;
    }
}

void drawFood() {
    putCursorOnMap(food);
    printChar(FOOD);
}

void generateFood() {
    do {
        food.x = rand() % MAP_WIDTH;
        food.y = rand() % MAP_HEIGHT;
    } while (food.x == snake[HEAD_INDEX].x && food.y == snake[HEAD_INDEX].y);
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

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
