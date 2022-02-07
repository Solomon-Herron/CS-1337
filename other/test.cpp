#include<stdio.h>
#include<iostream>

using namespace std;

int main(){
    const int SIZE = 10;
    int arr[SIZE] = {1, 2, 3, 4, 5};
    for (int i = 1; i < SIZE; i++){
        cout << arr[i];

    }
    cout << endl << arr[9] +1;

    return 0;
}