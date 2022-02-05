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
int snaketailNodesX[maxTailSize], snaketailNodesY[maxTailSize]; 
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
void getInput();
void increaseTailSize(int);
void changeSnakeDirection();
void checkIfSnakeHitBorder();
void checkIfSnakeAteTail();
void checkIfSnakeAteFruit();
void moveFruit();
void moveSnake();
void setup();
void shiftTail();

int main(){
    setup();
    while (!gameOver){
        drawGameBoard();
        getInput();
        moveSnake();
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

void getInput(){
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
        if (snaketailNodesX[i] == xCoordinate && snaketailNodesY[i] == yCoordinate){ 
            cout << "o";
            tailPrinted = true;
        }
    }
    if (!tailPrinted){
        cout << " ";
    }
}

void moveSnake(){
    shiftTail();
    changeSnakeDirection();
} 

void shiftTail(){
    //mental mapping names, fix. 
    int oldNodeCoordX = snaketailNodesX[0];  //initialize to value of first tail node 
    int oldNodeCoordY = snaketailNodesY[0];
    snaketailNodesX[0] = snakeHeadXcoordinate; // shift first node to head posi
    snaketailNodesY[0] = snakeHeadYcoordinate;
    int coordinatesOfNthNodeX, coordinatesOfNthNodeY;
    for (int i = 1; i < snakeTailSize; i++){ //shift all subsequent nodes to the position of the node ahead of them. 
        coordinatesOfNthNodeX = snaketailNodesX[i]; 
        coordinatesOfNthNodeY = snaketailNodesY[i];
        snaketailNodesX[i] = oldNodeCoordX; 
        snaketailNodesY[i] = oldNodeCoordY;
        oldNodeCoordX = coordinatesOfNthNodeX; 
        oldNodeCoordY = coordinatesOfNthNodeY;
    }
}

void changeSnakeDirection(){
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
    checkIfSnakeHitBorder();
    checkIfSnakeAteTail();
    checkIfSnakeAteFruit();
}

void checkIfSnakeHitBorder(){
    //head hits border -> appears on other side of board
    if (snakeHeadXcoordinate >= width -1){
        snakeHeadXcoordinate = 0; 
    } else if (snakeHeadXcoordinate < 0){
        snakeHeadXcoordinate = width - 1;
    }
    if (snakeHeadYcoordinate >= height){
        snakeHeadYcoordinate = 0; 
    } else if (snakeHeadYcoordinate < 0){
        snakeHeadYcoordinate = height - 1;
    }  
}

void checkIfSnakeAteTail(){
    //head hits tail -> game over
    for (int i = 0; i < snakeTailSize; i++){
        if (snaketailNodesX[i] == snakeHeadXcoordinate && snaketailNodesY[i] == snakeHeadYcoordinate)
            gameOver = true;
    }
}

void checkIfSnakeAteFruit(){
    //head hits fruit -> add points & increase tail size
    if (snakeHeadXcoordinate == fruitXcoordinate && snakeHeadYcoordinate == fruitYcoordinate){
        addPointsToScore(10);
        moveFruit();
        increaseTailSize(1);
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