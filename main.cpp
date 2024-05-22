#include <iostream>
#include <cstring>
#include <windows.h>
#define SCREEN_MAX_WIDTH 135
#define SCREEN_WIDTH 100
#define LEFT_BORDER_WIDTH 15
using namespace std;

// can overload this function for no arguments
void printLine(const char *s){
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)186;
    int n=strlen(s);
    cout<<s;
    for(int i=0;i<SCREEN_WIDTH-2-n;i++) cout<<" ";
    cout<<(char)186<<"\n";
}

void printScreen(){
    system("cls");

    // top line
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)201;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)187<<"\n";

    // body
    printLine("\0");
    printLine("\0");
    printLine("\0");
    printLine("\0");
    printLine("\0");
    printLine("\0");
    printLine("\0");
    printLine("\0");

    // bottom line
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)200;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)188<<"\n";
}

int main(){
    printScreen();
    return 0;
}