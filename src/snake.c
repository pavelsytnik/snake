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
#define TAIL_INDEX (snake_length - 1)

#define BODY_EXISTS (snake_length > 1)

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

int gameOver;

pos_t snake[SNAKE_ARRAY_SIZE];
int snake_length;
int tail_index;
int move_direction;

pos_t food;
int grow;

void setCursor(int x, int y);
void hideCursor();

void init();
void input();
void gameLoop();
void update();

void drawSnake();
void drawHead();
void drawMap();
void drawFood();

void printChar(char ch);
void repeatChar(char ch, int count);
void putCursorOnMap(pos_t pos);

void generateFood();
void eat();

void move();
void moveHead();
void eraseTail();

int wasCollision();

int areEqual(pos_t a, pos_t b);

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

    gameOver = 0;

    move_direction = RIGHT;
    snake_length = 3;

    snake[HEAD_INDEX].x = MAP_WIDTH / 2;
    snake[HEAD_INDEX].y = MAP_HEIGHT / 2;
    snake[1].x = snake[HEAD_INDEX].x - 1;
    snake[1].y = snake[HEAD_INDEX].y;
    snake[2].x = snake[HEAD_INDEX].x - 2;
    snake[2].y = snake[HEAD_INDEX].y;

    srand(time(NULL));
    generateFood();
    grow = 0;
}

void gameLoop() {
    while (!gameOver) {
        Sleep(100);
        input();
        update();
    }
}

int wasCollision() {
    int i;
    for (i = 1; i < snake_length; ++i) {
        if (areEqual(snake[HEAD_INDEX], snake[i])) {
            return 1;
        }
    }
    return 0;
}

void update() {
    move();
    if (grow) grow = 0;
    if (wasCollision()) {
        gameOver = 1;
    }
    if (areEqual(food, snake[HEAD_INDEX])) {
        eat();
    }
}

void move() {
    int i;
    eraseTail();
    for (i = TAIL_INDEX; i > HEAD_INDEX; i--) {
        snake[i] = snake[i - 1];
    }
    moveHead();
}

void moveHead() {
    if (BODY_EXISTS) {
        putCursorOnMap(snake[HEAD_INDEX]);
        printChar(SNAKE_BODY);
    }

    switch (move_direction) {
        case UP:
            if (snake[HEAD_INDEX].y == 0)
                snake[HEAD_INDEX].y = MAP_HEIGHT - 1;
            else
                snake[HEAD_INDEX].y--;
            break;
        case DOWN:
            if (snake[HEAD_INDEX].y == MAP_HEIGHT - 1)
                snake[HEAD_INDEX].y = 0;
            else
                snake[HEAD_INDEX].y++;
            break;
        case LEFT:
            if (snake[HEAD_INDEX].x == 0)
                snake[HEAD_INDEX].x = MAP_WIDTH - 1;
            else
                snake[HEAD_INDEX].x--;
            break;
        case RIGHT:
            if (snake[HEAD_INDEX].x == MAP_WIDTH - 1)
                snake[HEAD_INDEX].x = 0;
            else
                snake[HEAD_INDEX].x++;
            break;
    }

    drawHead();
}

void eraseTail() {
    if (!BODY_EXISTS || grow)
        return;

    putCursorOnMap(snake[TAIL_INDEX]);
    printChar(AIR);
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

void eat() {
    grow = 1;
    snake_length++;
    generateFood();
    drawFood();
}

void drawMap() {
    int i;

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
    for (i = 1; i < snake_length; ++i) {
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
    } while (areEqual(food, snake[HEAD_INDEX]));
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

int areEqual(pos_t a, pos_t b) {
    return a.x == b.x && a.y == b.y;
}
