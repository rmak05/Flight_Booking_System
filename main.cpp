#include <iostream>
#include <cstring>
#include <windows.h>
#define SCREEN_MAX_WIDTH 135
#define SCREEN_WIDTH 120
#define LEFT_BORDER_WIDTH 8
#define TEXT_PADDING 1
using namespace std;

enum align{
    left   = 0,
    right  = 1,
    center = 2
};

void printLine(){
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)186;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<" ";
    cout<<(char)186<<"\n";
}

void printLine(const char *s){
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)186;
    int n=strlen(s);
    cout<<s;
    for(int i=0;i<SCREEN_WIDTH-2-n;i++) cout<<" ";
    cout<<(char)186<<"\n";
}

void printLine(const char *s,const int a){
    int n=strlen(s),left_space=1,right_space=1;
    switch(a){
        case align::left :
            left_space=TEXT_PADDING;
            right_space=SCREEN_WIDTH-2-n-TEXT_PADDING;
            break;
        case align::right :
            left_space=SCREEN_WIDTH-2-n-TEXT_PADDING;
            right_space=TEXT_PADDING;
            break;
        case align::center :
            left_space=TEXT_PADDING+(SCREEN_WIDTH-2-n-2*TEXT_PADDING)/2;
            right_space=2*TEXT_PADDING+SCREEN_WIDTH-2-n-2*TEXT_PADDING-left_space;
            break;
        default :               // default is left
            left_space=TEXT_PADDING;
            right_space=SCREEN_WIDTH-2-n-TEXT_PADDING;
            break;
    }
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)186;
    for(int i=0;i<left_space;i++) cout<<" ";
    cout<<s;
    for(int i=0;i<right_space;i++) cout<<" ";
    cout<<(char)186<<"\n";
}

void printScreen(){
    system("cls");
    char text[SCREEN_WIDTH+10];

    // top line
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)201;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)187<<"\n";

    // title
    strcpy(text,"______ _ _       _     _     ______             _    _                _____           _                 ");
    printLine(text,align::center);
    strcpy(text,"|  ___| (_)     | |   | |    | ___ \\           | |  (_)              /  ___|         | |                ");
    printLine(text,align::center);
    strcpy(text,"| |_  | |_  __ _| |__ | |_   | |_/ / ___   ___ | | ___ _ __   __ _   \\ `--. _   _ ___| |_ ___ _ __ ___  ");
    printLine(text,align::center);
    strcpy(text,"|  _| | | |/ _` | '_ \\| __|  | ___ \\/ _ \\ / _ \\| |/ / | '_ \\ / _` |   `--. \\ | | / __| __/ _ \\ '_ ` _ \\ ");
    printLine(text,align::center);
    strcpy(text,"| |   | | | (_| | | | | |_   | |_/ / (_) | (_) |   <| | | | | (_| |  /\\__/ / |_| \\__ \\ ||  __/ | | | | |");
    printLine(text,align::center);
    strcpy(text,"\\_|   |_|_|\\__, |_| |_|\\__|  \\____/ \\___/ \\___/|_|\\_\\_|_| |_|\\__, |  \\____/ \\__, |___/\\__\\___|_| |_| |_|");
    printLine(text,align::center);
    strcpy(text,"            __/ |                                             __/ |          __/ |                      ");
    printLine(text,align::center);
    strcpy(text,"           |___/                                             |___/          |___/                       "); 
    printLine(text,align::center);

    // body
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();

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

// ascii art
// big
// doom (currently using)
// standard
// flight booking system 

//      _    _      _ _            
//     / \  (_)_ __| (_)_ __   ___ 
//    / _ \ | | '__| | | '_ \ / _ \
//   / ___ \| | |  | | | | | |  __/
//  /_/   \_\_|_|  |_|_|_| |_|\___|

//   ___  _      _ _            
//  / _ \(_)    | (_)           
// / /_\ \_ _ __| |_ _ __   ___ 
// |  _  | | '__| | | '_ \ / _ \
// | | | | | |  | | | | | |  __/
// \_| |_/_|_|  |_|_|_| |_|\___|
                             
//   ___  ___________ _     _____ _   _  _____ 
//  / _ \|_   _| ___ \ |   |_   _| \ | ||  ___|
// / /_\ \ | | | |_/ / |     | | |  \| || |__  
// |  _  | | | |    /| |     | | | . ` ||  __| 
// | | | |_| |_| |\ \| |_____| |_| |\  || |___ 
// \_| |_/\___/\_| \_\_____/\___/\_| \_/\____/   