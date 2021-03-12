#include <iostream>
#include <raylib.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <string>

struct Border{
    Rectangle left, right, up, down;
}borders;

//------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------
const static size_t width = 800;
const static size_t height = 600;
const static size_t cellSize = 20;

static bool gameOver = false;
static Rectangle food = {0, 0, cellSize, cellSize};
static Vector2 velocity = {cellSize, 0};
static Rectangle sBlock = {(10 * cellSize), (10 * cellSize), cellSize, cellSize};
static std::vector<Rectangle> sBody;
static size_t score = 0;
//------------------------------------------------------------------------
// Functions declaration
//------------------------------------------------------------------------
void initGame();
void drawGame();
void drawGrid();
void snakeInput();
void updateSnakePos();
void setBorders();
void drawBorders();
void drawSnake();
void spawnFood();
void restart();
bool wallHit();
bool tailEaten();
bool foodEaten();
//------------------------------------------------------------------------
// Main function's body
//------------------------------------------------------------------------
int main()
{
    initGame();
    while(!WindowShouldClose()){
        if (!gameOver){
            snakeInput();
            updateSnakePos();
            gameOver = wallHit();
            if (gameOver == false)
                gameOver = tailEaten();
            if (foodEaten()){
                sBody.push_back(sBody.at(0));
                spawnFood();
            }
            drawSnake();

            drawGame();
        } else{
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("GAME OVER!", 150, 200 , 80 , RED);
            std::string strScore ="Score: " + std::to_string(score);
            DrawText(strScore.c_str(), 310, 300 , 40 , LIGHTGRAY);
            DrawText("press space to restart", 160, 350 , 40 , PINK);
            EndDrawing();
            if (IsKeyPressed(KEY_SPACE)){
                restart();
            }
        }
    }
    CloseWindow();
    return 0;
}
void initGame()
{
    sBody.push_back(sBlock);
    setBorders();
    spawnFood();
    srand(time(NULL));
    SetTargetFPS(15);
    InitWindow(width, height, "Snake");
}

void drawGame()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    drawGrid();
    drawBorders();
    DrawRectangleRec(food, RED);
    EndDrawing();
}

void snakeInput()
{
    float speed = cellSize;
    if(IsKeyPressed(KEY_UP) && velocity.y != speed){
        velocity = {0, -speed};
    }
    else if(IsKeyPressed(KEY_DOWN) && velocity.y != -speed){
        velocity = {0, speed};
    }
    else if(IsKeyPressed(KEY_LEFT) && velocity.x != speed){
        velocity = {-speed, 0};
    }
    else if(IsKeyPressed(KEY_RIGHT) && velocity.x != -speed){
        velocity = {speed, 0};
    }
}

void drawGrid()
{
    for(size_t i = 0; i < (width / cellSize); ++i){
        DrawLine((i * cellSize), 0, i * cellSize, height, LIGHTGRAY);
    }
    for(size_t i = 0; i < (height / cellSize); ++i){
        DrawLine(0, (i * cellSize), width, (i * cellSize), LIGHTGRAY);
    }
}

void setBorders()
{
    borders.right = {0,float(cellSize), float(cellSize), float(height - 2 * cellSize)};
    borders.left = {float(width - cellSize), float(cellSize),float(cellSize), float(height - 2 * cellSize)};
    borders.up = {0,0, float(width), float(cellSize)};
    borders.down = {0, float(height - cellSize), float(width), float(cellSize)};
}
void drawBorders()
{
    DrawRectangleRec(borders.up, SKYBLUE);
    DrawRectangleRec(borders.down, SKYBLUE);
    DrawRectangleRec(borders.left, SKYBLUE);
    DrawRectangleRec(borders.right, SKYBLUE);
}
void updateSnakePos()
{
    for (size_t i = sBody.size() - 1; i > 0; --i){
        sBody.at(i).x = sBody.at(i - 1).x;
        sBody.at(i).y = sBody.at(i - 1).y;
    }
    sBody.at(0).x += velocity.x;
    sBody.at(0).y += velocity.y;
}
void drawSnake()
{
    DrawRectangleRec(sBody.at(0), GREEN);
    for(size_t i = 1; i < sBody.size(); ++i){
        DrawRectangleRec(sBody.at(i), BROWN);
    }
}
bool wallHit()
{
    bool temp = false;
    if(CheckCollisionRecs(sBody.at(0), borders.down) ){
        temp = true;
    } else if(CheckCollisionRecs(sBody.at(0), borders.up) ){
        temp = true;
    } else if(CheckCollisionRecs(sBody.at(0), borders.left) ){
        temp = true;
    } else if(CheckCollisionRecs(sBody.at(0), borders.right) ){
        temp = true;
    }
    return temp;
}
bool tailEaten()
{
    bool temp = false;
    if (sBody.size() > 4){
        for(size_t i = 5; i < sBody.size(); ++i){
            if (CheckCollisionRecs(sBody.at(0), sBody.at(i) ) ){
                temp = true;
            }
        }
    }
    return temp;
}
bool foodEaten()
{
    bool temp = false;
    if(CheckCollisionRecs(sBody.at(0), food)){
        score++;
        temp = true;
    }
    return temp;
}

void spawnFood()
{
    bool check = false;
    do {
        float xFood = ((std::rand() % 38) + 1) * cellSize;
        float yFood = ((std::rand() % 28) + 1) * cellSize;
        if( !((food.x == xFood) && (food.y == yFood)) ){
            food = {xFood,yFood, float(cellSize), float(cellSize)};
            for(size_t i = 0; i < sBody.size(); ++i){
                if(CheckCollisionRecs(sBody.at(i), food)){
                    check = true;
                    break;
                } else{
                    check = false;
                }
            }
        }
    } while (check);
}
void restart()
{
    gameOver = false;
    score = 0;
    velocity = {cellSize, 0};
    sBody.clear();
    sBlock = {(10 * cellSize), (10 * cellSize), cellSize, cellSize};
    sBody.push_back(sBlock);
}
