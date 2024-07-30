#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define WINDOW_TITLE "Snake"
#define SCORE_STRING "Score: "

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

#define CONSOLE_WIDTH ((MAP_WIDTH + 2) * 2)
#define CONSOLE_HEIGHT (MAP_HEIGHT + 4)

#define HEAD_INDEX 0
#define TAIL_INDEX (snake_length - 1)

#define BODY_EXISTS (snake_length > 1)

#define SECOND 1000
#define FPS 10

// #define NEW_LINE() putchar('\n')

HANDLE hConsole;

SHORT down_pressed;
SHORT up_pressed;
SHORT left_pressed;
SHORT right_pressed;

typedef struct Pos {
    int x;
    int y;
} Pos;

bool grow;
bool game_over;

Pos snake[SNAKE_ARRAY_SIZE];
int snake_length;
int tail_index;
int move_direction;

Pos food;

int score;

void Init(void);
void Input(void);
void GameLoop(void);
void HandleEndOfGame(void);
void Update(void);

void GenerateFood(void);
void Eat(void);

void Move(void);
void MoveHead(void);

void DrawMap(void);
void DrawBar(void);
void DrawSnake(void);
void DrawHead(void);
void DrawFood(void);

void DrawBodyOnHead(void);
void EraseTail(void);
void UpdateScore(void);
void DeathAnimation(void);

void PrintChar(char ch);
void RepeatChar(char ch, int count);

void SetCursorPosition(int x, int y);
void PutCursorOnMap(Pos pos);
void HideCursor(void);

bool PosEqual(Pos a, Pos b);
bool CollisionOccurred(void);
bool FoodInsideSnake(void);

int main(void)
{
    Init();

    DrawMap();
    DrawBar();
    DrawSnake();
    DrawFood();

    GameLoop();
    HandleEndOfGame();

    return 0;
}

void Init(void)
{
    system("title " WINDOW_TITLE);

    char command[28];
    sprintf(command, "mode con cols=%d lines=%d", CONSOLE_WIDTH, CONSOLE_HEIGHT);
    system(command);

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HideCursor();

    game_over = false;
    grow = false;
    move_direction = RIGHT;
    snake_length = 3;
    score = 0;

    snake[0].x = MAP_WIDTH / 2;
    snake[0].y = MAP_HEIGHT / 2;
    snake[1].x = snake[HEAD_INDEX].x - 1;
    snake[1].y = snake[HEAD_INDEX].y;
    snake[2].x = snake[HEAD_INDEX].x - 2;
    snake[2].y = snake[HEAD_INDEX].y;

    srand(time(NULL));
    GenerateFood();
}

void Input(void)
{
    down_pressed = (GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S')) & 0x8001;
    up_pressed = (GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W')) & 0x8001;
    left_pressed = (GetAsyncKeyState(VK_LEFT) | GetAsyncKeyState('A')) & 0x8001;
    right_pressed = (GetAsyncKeyState(VK_RIGHT) | GetAsyncKeyState('D')) & 0x8001;

    switch (move_direction) {
        case UP:
        case DOWN:
            if (left_pressed)
                move_direction = LEFT;
            else if (right_pressed)
                move_direction = RIGHT;
            break;
        case LEFT:
        case RIGHT:
            if (up_pressed)
                move_direction = UP;
            else if (down_pressed)
                move_direction = DOWN;
            break;
    }
}

void GameLoop(void)
{
    while (!game_over) {
        Sleep(SECOND / FPS);
        Input();
        Update();
    }
}

void HandleEndOfGame(void)
{
    DeathAnimation();

    // Set cursor at the end of the output
    SetCursorPosition(0, CONSOLE_HEIGHT - 1);

    system("pause");
}

void Update(void)
{
    Move();
    if (grow)
        grow = false;
    if (CollisionOccurred())
        game_over = true;
    if (PosEqual(food, snake[HEAD_INDEX]))
        Eat();
}

void GenerateFood(void)
{
    do {
        food.x = rand() % MAP_WIDTH;
        food.y = rand() % MAP_HEIGHT;
    } while (FoodInsideSnake());
}

void Eat(void)
{
    grow = true;
    snake_length++;
    score += 10;
    UpdateScore();
    GenerateFood();
    DrawFood();
}

void Move(void)
{
    if (BODY_EXISTS) {
        if (!grow)
            EraseTail();
        for (int i = TAIL_INDEX; i > HEAD_INDEX; --i)
            snake[i] = snake[i - 1];
        DrawBodyOnHead();
    }
    MoveHead();
}

void MoveHead(void)
{
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

    DrawHead();
}

void DrawMap(void)
{
    int i;

    SetCursorPosition(0, 0);
    RepeatChar(WALL, MAP_WIDTH + 2);
    // NEW_LINE();

    for (i = 0; i < MAP_HEIGHT; ++i) {
        SetCursorPosition(0, i + 1);
        PrintChar(WALL);
        RepeatChar(AIR, MAP_WIDTH);
        PrintChar(WALL);
        // NEW_LINE();
    }

    SetCursorPosition(0, i + 1);
    RepeatChar(WALL, MAP_WIDTH + 2);
    // NEW_LINE();
}

void DrawBar(void)
{
    SetCursorPosition(0, CONSOLE_HEIGHT - 2);
    printf("%s%d", SCORE_STRING, score);
}

void DrawSnake(void)
{
    DrawHead();
    for (int i = 1; i < snake_length; ++i) {
        PutCursorOnMap(snake[i]);
        PrintChar(SNAKE_BODY);
    }
}

void DrawHead(void)
{
    PutCursorOnMap(snake[HEAD_INDEX]);
    switch (move_direction) {
        case UP:
            PrintChar(SNAKE_HEAD_UP);
            break;
        case RIGHT:
            PrintChar(SNAKE_HEAD_RIGHT);
            break;
        case DOWN:
            PrintChar(SNAKE_HEAD_DOWN);
            break;
        case LEFT:
            PrintChar(SNAKE_HEAD_LEFT);
            break;
    }
}

void DrawFood(void)
{
    PutCursorOnMap(food);
    PrintChar(FOOD);
}

void EraseTail(void)
{
    PutCursorOnMap(snake[TAIL_INDEX]);
    PrintChar(AIR);
}

void UpdateScore(void)
{
    SetCursorPosition(sizeof(SCORE_STRING) - 1, CONSOLE_HEIGHT - 2);
    printf("%d", score);
}

void DeathAnimation(void)
{
    Sleep(SECOND / FPS);
    for (int i = 1; i < snake_length; i++) {
        PutCursorOnMap(snake[i]);
        if (!PosEqual(snake[i], snake[HEAD_INDEX]))
            PrintChar('X');
        Sleep(SECOND / FPS);
    }
}

void DrawBodyOnHead(void)
{
    PutCursorOnMap(snake[HEAD_INDEX]);
    PrintChar(SNAKE_BODY);
}

void PrintChar(char ch)
{
    putchar(ch);
    putchar(' ');
}

void RepeatChar(char ch, int count)
{
    for (int i = 0; i < count; ++i)
        PrintChar(ch);
}

void SetCursorPosition(int x, int y)
{
    SetConsoleCursorPosition(hConsole, (COORD) { x, y });
}

void PutCursorOnMap(Pos pos)
{
    SetCursorPosition((pos.x + 1) * 2, pos.y + 1);
}

void HideCursor(void)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

bool PosEqual(Pos a, Pos b) {
    return a.x == b.x && a.y == b.y;
}

bool CollisionOccurred(void)
{
    for (int i = 1; i < snake_length; ++i)
        if (PosEqual(snake[HEAD_INDEX], snake[i]))
            return true;
    return false;
}

bool FoodInsideSnake(void)
{
    for (int i = HEAD_INDEX; i <= TAIL_INDEX; ++i)
        if (PosEqual(snake[i], food))
            return true;
    return false;
}
