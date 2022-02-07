/*
 SnakeGame - Orig from the Web
 Original code by N. Vitanovic
 see his YouTube video here: https://bit.ly/29WZ5Ml

 Project Name: Snake Game
 Author: Solomon Herron / N. Vitanovic (original author)
 Date  : 01/25/2022
 Course: CS 1337.012
 Purpose: The purpose of this application is to exersice clean coding practices by refactoring someone else's code. 
          The code has been refaction by using meaningful, domain specific names, modularizing existing functions,
          separating their responsibilities into smaller functions that handle only one task. Formatting has also 
          been updated to remain more inline with Google styling and vertical formatting has been rearraged as follows:
              -I have implemented a top-down vertical formatting scheme, where the level of abstraction decreases as you 
               the further into the program you get. This allows the reader to get the gist of the programs functionality
               rather quickly without becoming bogged down by details. Callee functions are declared immediately after
               the function that called it where possible.
              -Spaces between sections of code represent 'complete thoughts'. 
              -I do not know how to use classes so I cannot shorten this file at all. The length/number of functions is giving me
               anxiety so I have added function-banners to give the program some visual breathing space making it a bit easier to read since there are so many of them.
 
 TODO:
    -Separate checkIfSnakeHitBorder() into two different functions
    -Refactor newly added code. A lot of code was added on 02/06/22 and I created a lot of oversized functions. 
    -Add more robust debugging
    -Add unit test
    -Add 'trap' game mode
    -Keep improving shiftTail() algorithm
    -Get second opnion on using function prototypes vs pre-main function declarations
 Bugs:
    -Everytime the snake eats a fruit, the tail node appear at top left of board (0,0) instead of at the end of the snake
    -End of snake tail appears to cross right side of border.
 Changelog:
    01/25/22
        -Renamed several variables to give more consise/clear meaning, changed formating and indentation style to Google styling
    01/27/22 
        -Attempted to minimize funtion sizes by seperating concerns
    02/01/22
        -Attempted to refactor logic function, for specifically the algorathim pertaining to shifting the position of the snake
    02/04/22
        -Renamed the variables in shifting mechanic to give a clearer interpretation of the algorithm. 
        -Finished breaking up all functions into smaller more consise functions that handle one task
    02/05/22
        -Adding debugging tools
        -Adding new features as required by HW 01
        -Added capability of selecting the number of fruit on the board. Used an array to store positions of all fruit.
        -Prompted user with end game message
        -Added start new game prompt
        -Added instructions to the beginning of game
        -Added option to allow user to choose between game modes using strict borders.
*/
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;
const int MAXTAILSIZE = 100;
const int MAXNUMFRUIT = 10;
int fruitXcoordinates[MAXNUMFRUIT];
int fruitYcoordinates[MAXNUMFRUIT];
int snaketailNodesX[MAXTAILSIZE];
int snaketailNodesY[MAXTAILSIZE];
int snakeHeadXcoordinate;
int snakeHeadYcoordinate;
int snakeTailSize;
int numOfFruit; 
bool tailPrinted = false;
bool fruitPrinted = false;
bool strictBorders;
bool gameOver;
int score;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton direction;

void addPointsToScore(int);
void getPlayerConfigurations();
void displayScore();
void drawGameBoard();
void drawFruit(int, int);
void drawTopOrBottomBorder();
void drawSnakeTail(int, int);
void getInput();
int  getIntegerInput(string);
char getCharInput(string);
void greetPlayer();
void increaseTailSize(int);
void initializeVariables(int);
void changeSnakeDirection();
void checkIfSnakeHitBorder();
void checkIfSnakeAteTail();
void checkIfSnakeAteFruit();
void moveFruit(int);
void moveSnake();
void printBanner();
void printGameInfo();
void shiftTail();
void selectGameMode();
void showFlow(string);
void showValue(int, string);
void showValue(int[], string);

int main(){
    greetPlayer();
    getPlayerConfigurations();
    while (!gameOver){
        drawGameBoard();
        getInput();
        moveSnake();
        Sleep(1);
    }
    return 0;
}

//---------------------------------------------------------------------//
//------------------User Game Configurations---------------------------//
//---------------------------------------------------------------------// 

void getPlayerConfigurations(){
    //Ask player how many fruit they would like
    bool validInput = false;
    while(!validInput){
        cout << endl << "How many fruit would you like to play with?";
        numOfFruit = getIntegerInput("Please enter a whole number between 1 and 9: ");
        //check if input is valid
        if (numOfFruit < 1 || numOfFruit > 9){
            cout << endl << "Sorry, your selection is invalid." << endl << endl;
        } else { validInput = true; }
    }
    selectGameMode();
    initializeVariables(numOfFruit);
}

//---------------------------------------------------------------------//

int gameModeSelection;
void selectGameMode(){
    bool validInput = false;
    while(!validInput){
        cout << "|-----------------------------------------------------------------------------------------------|" << endl;
        cout << "Game mode select:" << endl;
        cout << "1. Strict Borders: You will lose the game if you run into any of the borders of the game board" << endl;
        cout << "2. No Borders: If you run into a border, the head of your snake will appear on the other side of the board" << endl;
        gameModeSelection = getIntegerInput("Enter the number of the mode you would like to play: ");
        //check if input is valid
        if (gameModeSelection > 2 || gameModeSelection < 1){
            cout << endl << "Sorry, your selection is invalid." << endl << endl;
        } else { validInput = true; }
    }

    if (gameModeSelection == 1){
        strictBorders = true;
    } else { strictBorders = false;}
} 

//---------------------------------------------------------------------//

void initializeVariables(int numOfFruit){
    gameOver = false;
    direction = STOP;
    score = 0;
    snakeTailSize = 0;
    //center head on board
    snakeHeadXcoordinate = WIDTH / 2;
    snakeHeadYcoordinate = HEIGHT / 2;
    //randomize fruit
    for (int i = 0; i < numOfFruit; i++){
        fruitXcoordinates[i] = rand() % WIDTH;
        fruitYcoordinates[i] = rand() % HEIGHT;
    }
    //clear snake tail values
    for (int i = 0; i < MAXTAILSIZE; i++){
        snaketailNodesX[i] = 0; 
        snaketailNodesY[i] = 0;
    }
}

//---------------------------------------------------------------------//
//-------------------------drawGameBoard()-----------------------------//
//---------------------------------------------------------------------// 

void drawGameBoard(){
    system("cls"); //Linux: system("clear")
    drawTopOrBottomBorder();
    //Cycle through each x and y coordinate of game board, printing either a snake tail, fruit, empty space, or left/right edge of board
    for (int yCoordinate = 0; yCoordinate < HEIGHT; yCoordinate++){
        for (int xCoordinate = 0; xCoordinate < WIDTH; xCoordinate++){
            if (xCoordinate == 0 || xCoordinate == WIDTH -1 ){ cout << "#"; } //Draws left/right edges of border
            if (yCoordinate == snakeHeadYcoordinate && xCoordinate == snakeHeadXcoordinate){
                cout << "O";
            } else {
                //Tail/Fruit only print if coordinates of loop match tail/fruit coordinate array values
                drawSnakeTail(xCoordinate, yCoordinate); 
                drawFruit(xCoordinate, yCoordinate);
                if (!tailPrinted){
                    if(!fruitPrinted){
                        cout << " ";
                    }
                }   
            } 
        }
        cout << endl;
    }
    drawTopOrBottomBorder();
    displayScore();
} 

//---------------------------------------------------------------------//

void drawSnakeTail(int xCoordinate, int yCoordinate){
    tailPrinted = false;
    for (int i = 0; i < snakeTailSize; i++){
        if (snaketailNodesX[i] == xCoordinate && snaketailNodesY[i] == yCoordinate){ 
            cout << "o";
            tailPrinted = true;
        }
    }
}

void drawFruit(int xCoordinate, int yCoordinate){
    fruitPrinted = false;
    for (int i = 0; i < numOfFruit; i++){
        if (yCoordinate == fruitYcoordinates[i] && xCoordinate == fruitXcoordinates[i]){
            cout << "F";
            fruitPrinted = true;
        }
    }
}

//---------------------------------------------------------------------//
//----------------------Snake Movement Logic---------------------------//
//--------------------------------------------------------------------//

void moveSnake(){
    shiftTail();
    changeSnakeDirection();
} 

void shiftTail(){
    //initialize 'oldNodeCoord' to value of first tail node 
    int oldNodeCoordX = snaketailNodesX[0];    
    int oldNodeCoordY = snaketailNodesY[0];
    // shift first tail node to head's position
    snaketailNodesX[0] = snakeHeadXcoordinate; 
    snaketailNodesY[0] = snakeHeadYcoordinate;
    //shift all subsequent nodes to the position of the node ahead of them.
    int coordinatesOfNthNodeX, coordinatesOfNthNodeY;
    for (int i = 1; i < snakeTailSize; i++){ 
        coordinatesOfNthNodeX = snaketailNodesX[i]; 
        coordinatesOfNthNodeY = snaketailNodesY[i];
        snaketailNodesX[i] = oldNodeCoordX; 
        snaketailNodesY[i] = oldNodeCoordY;
        oldNodeCoordX = coordinatesOfNthNodeX; 
        oldNodeCoordY = coordinatesOfNthNodeY;
    }
   
}

void changeSnakeDirection(){
    showFlow("Entering: change direction");
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
    checkIfSnakeAteFruit();
    checkIfSnakeAteTail();
}

//---------------------------------------------------------------------//
//------------------------Snake Eating Logic---------------------------//
//--------------------------------------------------------------------//

void checkIfSnakeHitBorder(){
    char newGame;
    //Strict mode: head hits border -> game over
    if(strictBorders){
        if (snakeHeadXcoordinate > WIDTH || snakeHeadXcoordinate < 0 || snakeHeadYcoordinate > HEIGHT || snakeHeadYcoordinate < 0){
            gameOver = true;
            cout << endl << "\t You have run into the border!";
            cout << endl << endl << "\t \t" << " YOU LOSE -_-" << endl << endl;
            cout << "Would you like to play another game? Y/N: ";
            cin >> newGame;
            if (newGame == 'Y'){
                main();
            } else {
                cout << endl << "Thank you for playing!";
            }
        }  
    } else {
        //Non-strict mode: head hits border -> appears on other side of board
        if (snakeHeadXcoordinate >= WIDTH -1){
            snakeHeadXcoordinate = 0; 
        } else if (snakeHeadXcoordinate < 0){
            snakeHeadXcoordinate = WIDTH - 1;
        }
        if (snakeHeadYcoordinate >= HEIGHT){
            snakeHeadYcoordinate = 0; 
        } else if (snakeHeadYcoordinate < 0){
            snakeHeadYcoordinate = HEIGHT - 1;
        } 
    } 
}

//---------------------------------------------------------------------//

void checkIfSnakeAteTail(){
    char newGame;
    //head hits tail -> game over
    for (int i = 0; i < snakeTailSize; i++){
        if (snaketailNodesX[i] == snakeHeadXcoordinate && snaketailNodesY[i] == snakeHeadYcoordinate){
            gameOver = true;
            //I'm only leaving these debugging function calls here in my final code because assignment description told me to do so
            showFlow("Entering: snake ate the tail");
            showValue(snaketailNodesX[i], "snake tail nodex");
            showValue(snaketailNodesY[i], "snake tail nodey");
            showValue(snakeHeadXcoordinate, "snake head x");
            showValue(snakeHeadYcoordinate, "snake head y");
            cout << endl << "\t You have eaten your own tail!";
            cout << endl << endl << "\t \t" << " YOU LOSE -_-" << endl << endl;
            cout << "Would you like to play another game? Y/N: ";
            cin >> newGame;
            if (newGame == 'Y'){
                main();
            } else {
                cout << endl << "Thank you for playing!";
            }
        }
            
    }
}

//---------------------------------------------------------------------//

void checkIfSnakeAteFruit(){
    showFlow("Entering: check if ate fruit");
    //head hits fruit -> add points & increase tail size
    for (int i = 0; i <= numOfFruit; i++){
        if (snakeHeadXcoordinate == fruitXcoordinates[i] && snakeHeadYcoordinate == fruitYcoordinates[i]){
            addPointsToScore(10);
            moveFruit(i);
            increaseTailSize(1);
    }
    }
    
}
//---------------------------------------------------------------------//
//---------------------low level helper functions---------------------//
//--------------------------------------------------------------------/

void moveFruit(int i){
    fruitXcoordinates[i] = rand() % WIDTH;
    fruitYcoordinates[i] = rand() % HEIGHT;
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
        case 'p':
            system("pause");
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

int getIntegerInput(string message){
    int input;
    cout << endl <<  message;
    cin >> input;
    return input;
}
char getCharInput(string message){
    char input;
    cout << endl << message;
    cin >> input;
    return input;
}

void increaseTailSize(int incrementAmount){
    snakeTailSize += incrementAmount;
}

void displayScore(){
    cout << "Score:" << score << endl;
}

void drawTopOrBottomBorder(){
    for (int i = 0; i < WIDTH+1; i++){
        cout << "#";
    }
    cout << endl;
}

void addPointsToScore(int points){
    score += points;
}

void greetPlayer(){
    printBanner();
    printGameInfo();
}

void printBanner(){
    cout << " _______  _        _______  _        _______" << endl;
    cout << "(  ____ \\( (    /|(  ___  )| \\    /\\(  ____ \\" << endl;
    cout << "| (    \\/|  \\  ( || (   ) ||  \\  / /| (    \\/" << endl;
    cout << "| (_____ |   \\ | || (___) ||  (_/ / | (__" << endl;
    cout << "(_____  )| (\\ \\) ||  ___  ||   _ (  |  __)" << endl;
    cout << "      ) || | \\   || (   ) ||  ( \\ \\ | (" << endl;
    cout << "/\\____) || )  \\  || )   ( ||  /  \\ \\| (____/\\" << endl;
    cout << "\\_______)|/    )_)|/     \\||_/    \\/(_______/" << endl;
}

void printGameInfo(){
    cout << endl;
    cout << "|-----------------------------------------------------------------------------------------------|" << endl;
    cout << "| -Use w/a/s/d keys to move your snake in the up, left, down, and right directions respectively.|" << endl;
    cout << "| -Press 'p' to pause, press 'x' to quit.                                                       |" << endl;
    cout << "| -Collect fruit (F) to earn points and increase your score.                                    |" << endl;
    cout << "| -As you eat fruit, your tail will increase in size.                                           |" << endl;
    cout << "| -Avoid eating your own tail! If you run into your tail, you lose the game.                    |" << endl;
    cout << "|-----------------------------------------------------------------------------------------------|" << endl;
}
//--------------------------------------------------------------------//
//--------------------Debugging tools--------------------------------//
//--------------------------------------------------------------------//

#define pause() system("pause")

bool showValueDebugging = false;
bool showflowDebugging = false;

void showFlow(string msg){
    if (showflowDebugging) {
        cout << "--- " << msg << endl;
        system("pause");
    }
}

void showValue(int var, string variableName){
    if (showValueDebugging){
        cout << "--- " << variableName << ": " << var << endl;
        system("pause"); 
    }
}

void showValue(int arr[], string variableName){
    if (showValueDebugging){
        cout << "--- " << variableName << ": " << endl;
        for (int i = 0; i < MAXTAILSIZE; i++){
            cout << arr[i];
        }
        system("pause"); 
    }
}



