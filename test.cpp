#include<stdio.h>
#include<iostream>

using namespace std;

int main(){
    const int SIZE = 5;
    int arr[SIZE] = {1, 2, 3, 4, 5};

    for (int i = 1; i < SIZE; i++){
        arr[i] = arr[i -1];

    }
    
    for (int i = 0; i < SIZE; i++)
    {
        cout << arr[i];
    }
    
    
    printf("yo");
    return 0;
}