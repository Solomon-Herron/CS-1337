// SnakeGame - Orig from the Web
// Original code by N. Vitanovic
// see his YouTube video here: https://bit.ly/29WZ5Ml

#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

int snakeHeadXcoordinate, snakeHeadYcoordinate;
int fruitXcoordinate, fruitYcoordinate;
const int maxTailSize = 100;
int tailNubsX[maxTailSize], snaketailNodesX[maxTailSize]; 
int snakeTailSize;

const int width = 20;
const int height = 20; 
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton direction;
bool gameOver;
int score;

void addPointsToScore(int);
void displayScore();
void drawGameBoard();
void drawTopOrBottomBorder();
void drawSnake(int, int);
void input();
void increaseTailSize(int);
void moveSnakeHead();
void moveFruit();
void logic();
void setup();
void shiftTail();

int main(){
    setup();
    while (!gameOver)
    {
        drawGameBoard();
        input();
        logic();
        Sleep(1);
    }
    return 0;
}

void setup(){
    gameOver = false;
    direction = STOP;
    snakeHeadXcoordinate = width / 2;
    snakeHeadYcoordinate = height / 2;
    fruitXcoordinate = rand() % width;
    fruitYcoordinate = rand() % height;
    score = 0;
}

void logic(){
    shiftTail();
    moveSnakeHead();
}

void drawGameBoard(){
    system("cls"); 
    drawTopOrBottomBorder();

    for (int yCoordinate = 0; yCoordinate < height; yCoordinate++){
        for (int xCoordinate = 0; xCoordinate < width; xCoordinate++){
            if (xCoordinate == 0 || xCoordinate == width -1 ){ cout << "#"; } //Draws left/right edges of border
            if (yCoordinate == snakeHeadYcoordinate && xCoordinate == snakeHeadXcoordinate){
                cout << "O";
            } else if (yCoordinate == fruitYcoordinate && xCoordinate == fruitXcoordinate){
                cout << "F";
            } else {
                drawSnake(xCoordinate, yCoordinate);   
            } 
        }
        cout << endl;
    }

    drawTopOrBottomBorder();
    displayScore();
} 

void drawSnake(int xCoordinate, int yCoordinate){
    bool tailPrinted = false;
    for (int i = 0; i < snakeTailSize; i++){
        if (tailNubsX[i] == xCoordinate && snaketailNodesX[i] == yCoordinate){ 
            cout << "o";
            tailPrinted = true;
        }
    }
    if (!tailPrinted){
        cout << " ";
    }
}

void shiftTail(){
    //mental mapping names, fix. 
    int prevX = tailNubsX[0]; 3
    int prevY = snaketailNodesX[0]; 6
    int prev2X, prev2Y;
    tailNubsX[0] = snakeHeadXcoordinate;  2
    snaketailNodesX[0] = snakeHeadYcoordinate;  5
    for (int i = 1; i < snakeTailSize; i++){
        prev2X = tailNubsX[i]; 4
        prev2Y = snaketailNodesX[i]; 7
        tailNubsX[i] = prevX; 3
        snaketailNodesX[i] = prevY; 6
        prevX = prev2X;  4
        prevY = prev2Y;  7
    }
}

void moveSnakeHead(){
    switch (direction) {
        case LEFT:
            snakeHeadXcoordinate--;
            break;
        case RIGHT:
            snakeHeadXcoordinate++;
            break;
        case UP:
            snakeHeadYcoordinate--;
            break;
        case DOWN:
            snakeHeadYcoordinate++;
            break;
        default:
            break;
    }

    //head hits border -> appears on other side of board
    if (snakeHeadXcoordinate >= width -1){
        
    } snakeHeadXcoordinate = 0; else if (snakeHeadXcoordinate < 0) snakeHeadXcoordinate = width - 1;
    if (snakeHeadYcoordinate >= height) snakeHeadYcoordinate = 0; else if (snakeHeadYcoordinate < 0) snakeHeadYcoordinate = height - 1;

    //head hits tail -> game over
    for (int i = 0; i < snakeTailSize; i++){
        if (tailNubsX[i] == snakeHeadXcoordinate && snaketailNodesX[i] == snakeHeadYcoordinate)
            gameOver = true;
    }
    
    //head hits fruit -> add points & increase tail size
    if (snakeHeadXcoordinate == fruitXcoordinate && snakeHeadYcoordinate == fruitYcoordinate){
        addPointsToScore(10);
        moveFruit();
        increaseTailSize(1);
    }
}

void input(){
    if (_kbhit()){
        switch (_getch()){
        case 'a':
            direction = LEFT;
            break;
        case 'd':
            direction = RIGHT;
            break;
        case 'w':
            direction = UP;
            break;
        case 's':
            direction = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void moveFruit(){
    fruitXcoordinate = rand() % width;
    fruitYcoordinate = rand() % height;
}

void increaseTailSize(int incrementAmount){
    snakeTailSize += incrementAmount;
}

void displayScore(){
    cout << "Score:" << score << endl;
}

void drawTopOrBottomBorder(){
    for (int i = 0; i < width+1; i++){
        cout << "#";
    }
    cout << endl;
}

void addPointsToScore(int points){
    score += points;
}