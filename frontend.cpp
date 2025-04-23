#include <iostream>
#include <fstream>
#include <windows.h>
#include <pthread.h>
#include <conio.h>
#include <limits>
#include <utility>
#include <vector>
#include <stack>
#include <cstring>
#include <iomanip>
#define SCREEN_MAX_WIDTH 135
#define SCREEN_WIDTH 120
#define LEFT_BORDER_WIDTH 7
#define TEXT_PADDING 10
#define IMPORTANT_UPDATES_TEXT_DELAY 100
#define IMP_UPDATES_WIDTH 75
#define SMALL_SIZE 3
#define MEDIUM_SIZE 50
#define LARGE_SIZE 100
#define ESCAPE 27
using namespace std;

// 'G' = Guest , 'U' = User , 'A' = Admin
unsigned char user_choice,user_type;
int curr_screen;

enum align{
    left   = 0,
    right  = 1,
    center = 2,
    custom = 3
};

enum line{
    dashed = 0,
    dotted = 1,
};

enum screen{
    usage_type             = (-1),
    guest_homepage         = 0,
    user_homepage          = 1,
    admin_homepage         = 2,
    program_exit           = 3,
    logged_out             = 4,
    manage_airports        = 5,
    add_airport            = 6,
    delete_airport         = 7,
    airport_list           = 8,
    manage_airlines        = 9,
    add_airline            = 10,
    delete_airline         = 11,
    airline_list           = 12,
    manage_airplane_models = 13,
    add_airplane_model     = 14,
    delete_airplane_model  = 15,
    airplane_model_list    = 16,
    manage_routes          = 17,
    add_route              = 18,
    delete_route           = 19,
    route_list             = 20,
    manage_airplanes       = 21,
    add_airplane           = 22,
    delete_airplane        = 23,
    airplane_list          = 24,
    manage_imp_updates     = 25,
    add_imp_update         = 26,
    delete_imp_update      = 27,
    imp_update_list        = 28,
    book_ticket            = 29,
    flight_availability    = 30
};

// source https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
enum colour{
    black    = 30,
    red      = 31,
    green    = 32,
    yellow   = 33,
    blue     = 34,
    magenta  = 35,
    cyan     = 36,
    white    = 37,
    _default = 39           // "default" is a keyword
};

void setForegroundColour(colour c){
    char sequence[15];
    sprintf(sequence,"\x1b[%dm",c);
    cout<<sequence;
}

void setForegroundColour(int r, int g, int b){
    if(r<0 || r>255 || g<0 || g>255 || b<0 || b>255) return;
    char sequence[15];
    sprintf(sequence,"\x1b[38;2;%d;%d;%dm",r,g,b);
    cout<<sequence;
}

void setBackgroundColour(){
    cout<<"\x1b[48;5;0m";       // black
}

void resetBackgorund(){
    cout<<"\x1b[49m";
}

void resetStyle(){
    cout<<"\x1b[0m";
}

struct circularListTextNode{
    char node_value;
    circularListTextNode *next;

    circularListTextNode(){
        node_value='\0';
        next=NULL;
    }

    circularListTextNode(char value){
        node_value=value;
        next=NULL;
    }
};

void setCursorVisibility(bool set){
    HANDLE output_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(output_handle,&cursor_info);
    cursor_info.bVisible=set;
    // cursor_info.dwSize=20;      // 20 is default cursor size
    SetConsoleCursorInfo(output_handle,&cursor_info);
}

// returns number of lines printed
int printLine(){
    int num_lines=0;
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<" ";
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

// returns number of lines printed
int printLine(line a){
    int num_lines=0;
    char line_ch;
    switch(a){
        case line::dashed :
            line_ch=(char)196;
            break;
        case line::dotted :
            line_ch=(char)250;
            break;
        default :               // default is empty line
            line_ch=' ';
            break;
    }
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    if(a==line::dashed) setForegroundColour(89,96,97);
    else if(a==line::dotted) setForegroundColour(132,139,140);
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<line_ch;
    setForegroundColour(colour::_default);
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

// returns number of lines printed
int printLine(const char *s, colour c){
    int num_lines=0;
    int n=strlen(s);
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    setForegroundColour(c);
    cout<<s;
    setForegroundColour(colour::_default);
    for(int i=0;i<SCREEN_WIDTH-TEXT_PADDING-2-n;i++) cout<<" ";
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

// returns number of lines printed
int printLine(const char *s1, const char *s2, colour c){
    int n1=strlen(s1),n2=strlen(s2),num_lines=0;
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    setForegroundColour(c);
    cout<<s1;
    setForegroundColour(colour::_default);
    for(int i=0;i<(SCREEN_WIDTH-2)/2-TEXT_PADDING-n1;i++) cout<<" ";
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    setForegroundColour(c);
    cout<<s2;
    setForegroundColour(colour::_default);
    for(int i=0;i<(SCREEN_WIDTH-2)/2-TEXT_PADDING-n2;i++) cout<<" ";
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

// use this function only to print text with serial number
// returns number of lines printed
int printLine(const char *s1, const char *s2, const char *s3, const char *s4, colour c, bool isSerial){
    if(!isSerial) return 0;
    int n1=strlen(s1),n2=strlen(s2),n3=strlen(s3),n4=strlen(s4),num_lines=0;
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    setForegroundColour(c);
    cout<<s1;
    setForegroundColour(colour::_default);
    setForegroundColour(255,215,0);
    cout<<s2;
    setForegroundColour(colour::_default);
    for(int i=0;i<(SCREEN_WIDTH-2)/2-TEXT_PADDING-n1-n2;i++) cout<<" ";
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    setForegroundColour(c);
    cout<<s3;
    setForegroundColour(colour::_default);
    setForegroundColour(255,215,0);
    cout<<s4;
    setForegroundColour(colour::_default);
    for(int i=0;i<(SCREEN_WIDTH-2)/2-TEXT_PADDING-n3-n4;i++) cout<<" ";
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

// use this function only to print text with serial number
// returns number of lines printed
int printLine(const char *s1, const char *s2, colour c, bool isSerial){
    if(!isSerial) return printLine(s1,s2,c);
    int num_lines=0;
    int n1=strlen(s1),n2=strlen(s2);
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    setForegroundColour(c);
    cout<<s1;
    setForegroundColour(colour::_default);
    setForegroundColour(255,215,0);
    cout<<s2;
    setForegroundColour(colour::_default);
    for(int i=0;i<SCREEN_WIDTH-TEXT_PADDING-2-n1-n2;i++) cout<<" ";
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

// returns number of lines printed
int printLine(const char *s, align a, colour c){
    int n=strlen(s),left_space=1,right_space=1,num_lines=0;
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
            left_space=(SCREEN_WIDTH-2-n)/2;
            right_space=SCREEN_WIDTH-2-n-left_space;
            break;
        default :               // default is left
            left_space=TEXT_PADDING;
            right_space=SCREEN_WIDTH-2-n-TEXT_PADDING;
            break;
    }
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    for(int i=0;i<left_space;i++) cout<<" ";
    setForegroundColour(c);
    cout<<s;
    setForegroundColour(colour::_default);
    for(int i=0;i<right_space;i++) cout<<" ";
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

// use this function only to print text with serial number
// returns number of lines printed
int printLine(const char *s1, const char *s2, align a, colour c, bool isSerial){
    if(!isSerial) return printLine(s1,s2,c);
    int n1=strlen(s1),n2=strlen(s2),left_space=1,right_space=1,num_lines=0;
    switch(a){
        case align::left :
            left_space=TEXT_PADDING;
            right_space=SCREEN_WIDTH-2-n1-n2-TEXT_PADDING;
            break;
        case align::right :
            left_space=SCREEN_WIDTH-2-n1-n2-TEXT_PADDING;
            right_space=TEXT_PADDING;
            break;
        case align::center :
            left_space=(SCREEN_WIDTH-2-n1-n2)/2;
            right_space=SCREEN_WIDTH-2-n1-n2-left_space;
            break;
        default :               // default is left
            left_space=TEXT_PADDING;
            right_space=SCREEN_WIDTH-2-n1-n2-TEXT_PADDING;
            break;
    }
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    for(int i=0;i<left_space;i++) cout<<" ";
    setForegroundColour(c);
    cout<<s1;
    setForegroundColour(colour::_default);
    setForegroundColour(255,215,0);
    cout<<s2;
    setForegroundColour(colour::_default);
    for(int i=0;i<right_space;i++) cout<<" ";
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

// desired_text_padding will by default be taken as left padding unless it is for align::right
// desired_text_padding is ignored for align::center
// foreground colour is fixed
// returns number of lines printed
int printLine(const char *s, align a, const int desired_text_padding){
    int n=strlen(s),left_space=1,right_space=1,num_lines=0;
    switch(a){
        case align::left :
            left_space=desired_text_padding;
            right_space=SCREEN_WIDTH-2-n-desired_text_padding;
            break;
        case align::right :
            left_space=SCREEN_WIDTH-2-n-desired_text_padding;
            right_space=desired_text_padding;
            break;
        case align::center :
            left_space=(SCREEN_WIDTH-2-n)/2;
            right_space=SCREEN_WIDTH-2-n-left_space;
            break;
        case align::custom :
            left_space=desired_text_padding;
            right_space=SCREEN_WIDTH-2-n-left_space;
            break;
        default :               // default is left
            left_space=desired_text_padding;
            right_space=SCREEN_WIDTH-2-n-desired_text_padding;
            break;
    }
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    for(int i=0;i<left_space;i++) cout<<" ";
    setForegroundColour(100,0,255);
    cout<<s;
    setForegroundColour(colour::_default);
    for(int i=0;i<right_space;i++) cout<<" ";
    setForegroundColour(70,224,227);
    cout<<(char)186;
    setForegroundColour(colour::_default);
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    return num_lines;
}

circularListTextNode* makeTextCircular(const char *s, const int imp_updates_width){
    circularListTextNode *head=NULL,*temp=NULL;
    head = new circularListTextNode(' ');
    temp=head;
    for(int i=0;i<imp_updates_width-1;i++){
        temp->next = new circularListTextNode(' ');
        temp=temp->next;
    }
    for(int i=0;s[i]!='\0';i++){
        temp->next = new circularListTextNode(s[i]);
        temp=temp->next;
    }
    temp->next=head;
    return head;
}

// returns number of lines printed
int printTitle(){
    int num_lines=0;
    char text[SCREEN_WIDTH];
    // ascii art taken from patorjk.com
    strcpy(text,"______ _ _       _     _     ______             _    _                _____           _                 ");
    printLine(text,align::center,colour::blue);
    num_lines++;
    strcpy(text,"|  ___| (_)     | |   | |    | ___ \\           | |  (_)              /  ___|         | |                ");
    printLine(text,align::center,colour::blue);
    num_lines++;
    strcpy(text,"| |_  | |_  __ _| |__ | |_   | |_/ / ___   ___ | | ___ _ __   __ _   \\ `--. _   _ ___| |_ ___ _ __ ___  ");
    printLine(text,align::center,colour::blue);
    num_lines++;
    strcpy(text,"|  _| | | |/ _` | '_ \\| __|  | ___ \\/ _ \\ / _ \\| |/ / | '_ \\ / _` |   `--. \\ | | / __| __/ _ \\ '_ ` _ \\ ");
    printLine(text,align::center,colour::blue);
    num_lines++;
    strcpy(text,"| |   | | | (_| | | | | |_   | |_/ / (_) | (_) |   <| | | | | (_| |  /\\__/ / |_| \\__ \\ ||  __/ | | | | |");
    printLine(text,align::center,colour::blue);
    num_lines++;
    strcpy(text,"\\_|   |_|_|\\__, |_| |_|\\__|  \\____/ \\___/ \\___/|_|\\_\\_|_| |_|\\__, |  \\____/ \\__, |___/\\__\\___|_| |_| |_|");
    printLine(text,align::center,colour::blue);
    num_lines++;
    strcpy(text,"            __/ |                                             __/ |          __/ |                      ");
    printLine(text,align::center,colour::blue);
    num_lines++;
    strcpy(text,"           |___/                                             |___/          |___/                       "); 
    printLine(text,align::center,colour::blue);
    num_lines++;
    return num_lines;
}

// returns number of lines printed
int printTopBorder(){
    setForegroundColour(70,224,227);
    int num_lines=0;
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    cout<<(char)201;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)187;
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    setForegroundColour(colour::_default);
    return num_lines;
}

// returns number of lines printed
int printBottomBorder(){
    setForegroundColour(70,224,227);
    int num_lines=0;
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    setBackgroundColour();
    cout<<(char)200;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)188;
    resetBackgorund();
    cout<<"\n";
    num_lines++;
    setForegroundColour(colour::_default);
    return num_lines;
}

char* getAllImpUpdates(){
    int list_size,req_size,curr_size=0;
    fstream imp_updates_file;
    vector<char*> updates_list;
    char *text = new char[2*LARGE_SIZE+1];
    imp_updates_file.open("Data/important_updates_data.bin",ios::in | ios::app | ios::binary);
    while(imp_updates_file.read((char*)text,2*LARGE_SIZE+1)){
        req_size+=strlen(text);
        updates_list.push_back(text);
        text = new char[2*LARGE_SIZE+1];
    }
    imp_updates_file.close();
    list_size=updates_list.size();
    char *final_text = new char[req_size+1];
    final_text[0]='\0';
    for(int i=0;i<list_size;i++){
        if(i!=0){
            strcpy(final_text+curr_size," | ");
            curr_size+=sizeof(" | ")-1;
        }
        strcpy(final_text+curr_size,updates_list[i]);
        curr_size+=strlen(updates_list[i]);
    }
    return final_text;
}

void* guestScreen(void *p){
    system("cls");
    int imp_news_line=0;

    imp_news_line+=printTopBorder();
    imp_news_line+=printTitle();
    printLine();
    imp_news_line++;
    printLine();
    imp_news_line++;
    printLine(line::dashed);
    imp_news_line++;
    printLine();
    imp_news_line++;

    // important updates
    char imp_updates_title_text[]="Important Updates : ";
    char *imp_text=getAllImpUpdates();
    int imp_updates_title_text_size=strlen(imp_updates_title_text);
    int desired_left_space=TEXT_PADDING;
    printLine(imp_updates_title_text,align::custom,desired_left_space);      

    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","View Airport List","2. ","Check flight availability",colour::green,true);
    printLine("3. ","Log out",colour::green,true);
    printLine();
    printLine();
    printLine();
    printBottomBorder();

    // important updates
    HANDLE imp_updates_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD imp_updates_coord;
    imp_updates_coord.X=LEFT_BORDER_WIDTH+1+desired_left_space+imp_updates_title_text_size;
    imp_updates_coord.Y=imp_news_line;
    circularListTextNode *imp_text_ll=makeTextCircular(imp_text,IMP_UPDATES_WIDTH),*temp_ll;
    user_choice='\0';
    while(true){
        BOOL handle_success=SetConsoleCursorPosition(imp_updates_handle,imp_updates_coord);
        if(!handle_success) continue;
        temp_ll=imp_text_ll;
        for(int i=0;i<IMP_UPDATES_WIDTH;i++){
            setBackgroundColour();
            setForegroundColour(colour::red);
            cout<<(temp_ll->node_value);
            setForegroundColour(colour::_default);
            resetBackgorund();
            temp_ll=temp_ll->next;
        }
        imp_text_ll=imp_text_ll->next;
        cout<<"\n\n\n\n\n\n\n\n\n\n\n";
        Sleep(IMPORTANT_UPDATES_TEXT_DELAY);
        if(user_choice!='\0' || curr_screen!=guest_homepage) break;
    }

    return NULL;
}

void* printUserScreen(void *p){
    system("cls");
    int imp_news_line=0;

    imp_news_line+=printTopBorder();
    imp_news_line+=printTitle();
    printLine();
    imp_news_line++;
    printLine();
    imp_news_line++;
    printLine(line::dashed);
    imp_news_line++;
    printLine();
    imp_news_line++;

    // important updates
    char imp_updates_title_text[]="Important Updates : ";
    char *imp_text=getAllImpUpdates();
    int imp_updates_title_text_size=strlen(imp_updates_title_text);
    int desired_left_space=TEXT_PADDING;
    printLine(imp_updates_title_text,align::custom,desired_left_space);      

    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","Open profile","2. ","View Airport List",colour::green,true);
    printLine("3. ","Check flight availability","4. ","Book ticket",colour::green,true);
    printLine("5. ","Log out",colour::green,true);
    printLine();
    printLine();
    printLine();
    printBottomBorder();

    // important updates
    HANDLE imp_updates_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD imp_updates_coord;
    imp_updates_coord.X=LEFT_BORDER_WIDTH+1+desired_left_space+imp_updates_title_text_size;
    imp_updates_coord.Y=imp_news_line;
    circularListTextNode *imp_text_ll=makeTextCircular(imp_text,IMP_UPDATES_WIDTH),*temp_ll;
    user_choice='\0';
    while(true){
        BOOL handle_success=SetConsoleCursorPosition(imp_updates_handle,imp_updates_coord);
        if(!handle_success) continue;
        temp_ll=imp_text_ll;
        for(int i=0;i<IMP_UPDATES_WIDTH;i++){
            setBackgroundColour();
            setForegroundColour(colour::red);
            cout<<(temp_ll->node_value);
            setForegroundColour(colour::_default);
            resetBackgorund();
            temp_ll=temp_ll->next;
        }
        imp_text_ll=imp_text_ll->next;
        cout<<"\n\n\n\n\n\n\n\n\n\n\n";
        Sleep(IMPORTANT_UPDATES_TEXT_DELAY);
        if(user_choice!='\0' || curr_screen!=user_homepage) break;
    }

    return NULL;
}

void* printAdminScreen(void *p){
    system("cls");
    int imp_news_line=0;

    imp_news_line+=printTopBorder();
    imp_news_line+=printTitle();
    printLine();
    imp_news_line++;
    printLine();
    imp_news_line++;
    printLine(line::dashed);
    imp_news_line++;
    printLine();
    imp_news_line++;

    // important updates
    char imp_updates_title_text[]="Important Updates : ";
    char *imp_text=getAllImpUpdates();
    int imp_updates_title_text_size=strlen(imp_updates_title_text);
    int desired_left_space=TEXT_PADDING;
    printLine(imp_updates_title_text,align::custom,desired_left_space);      

    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","Manage Important Updates","2. ","Manage Airports",colour::green,true);
    printLine("3. ","Manage Airlines","4. ","Manage Airplane Models",colour::green,true);
    printLine("5. ","Manage Routes","6. ","Manage Flights",colour::green,true);
    printLine("7. ","Log out",colour::green,true);
    printLine();
    printLine();
    printBottomBorder();

    // important updates
    HANDLE imp_updates_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD imp_updates_coord;
    imp_updates_coord.X=LEFT_BORDER_WIDTH+1+desired_left_space+imp_updates_title_text_size;
    imp_updates_coord.Y=imp_news_line;
    circularListTextNode *imp_text_ll=makeTextCircular(imp_text,IMP_UPDATES_WIDTH),*temp_ll;
    user_choice='\0';
    while(true){
        BOOL handle_success=SetConsoleCursorPosition(imp_updates_handle,imp_updates_coord);
        if(!handle_success) continue;
        temp_ll=imp_text_ll;
        for(int i=0;i<IMP_UPDATES_WIDTH;i++){
            setBackgroundColour();
            setForegroundColour(colour::red);
            cout<<(temp_ll->node_value);
            setForegroundColour(colour::_default);
            resetBackgorund();
            temp_ll=temp_ll->next;
        }
        imp_text_ll=imp_text_ll->next;
        cout<<"\n\n\n\n\n\n\n\n\n\n\n";
        Sleep(IMPORTANT_UPDATES_TEXT_DELAY);
        if(user_choice!='\0' || curr_screen!=admin_homepage) break;
    }

    return NULL;
}

void* printUserTypeSelectionScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","Guest",align::center,colour::green,true);
    printLine("2. ","User",align::center,colour::green,true);
    printLine("3. ","Admin",align::center,colour::green,true);
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void* printLoggedOutScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("Successfully Logged Out",align::center,colour::green);
    printLine("Press any key to continue ...",align::center,colour::green);
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void* takeInput(void *p){
    setCursorVisibility(false);
    user_choice=(unsigned char)getch();
    if(curr_screen==screen::usage_type){
        if(user_choice=='1'){
            user_type='G';
            curr_screen=screen::guest_homepage;
        }
        else if(user_choice=='2'){
            user_type='U';
            curr_screen=screen::user_homepage;
        }
        else if(user_choice=='3'){
            user_type='A';
            curr_screen=screen::admin_homepage;
        }
        else if(user_choice==(char)ESCAPE) curr_screen=screen::program_exit;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::guest_homepage){
        if(user_choice=='1') curr_screen=screen::airport_list;
        else if(user_choice=='2') curr_screen=screen::flight_availability;
        else if(user_choice=='3') curr_screen=screen::logged_out;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::user_homepage){
        if(user_choice=='1'){
            
        }
        else if(user_choice=='2') curr_screen=screen::airport_list;
        else if(user_choice=='3') curr_screen=screen::flight_availability;
        else if(user_choice=='4') curr_screen=screen::book_ticket;
        else if(user_choice=='5') curr_screen=screen::logged_out;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::admin_homepage){
        if(user_choice=='1') curr_screen=screen::manage_imp_updates;
        else if(user_choice=='2') curr_screen=screen::manage_airports;
        else if(user_choice=='3') curr_screen=screen::manage_airlines;
        else if(user_choice=='4') curr_screen=screen::manage_airplane_models;
        else if(user_choice=='5') curr_screen=screen::manage_routes;
        else if(user_choice=='6') curr_screen=screen::manage_airplanes;
        else if(user_choice=='7') curr_screen=screen::logged_out;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::manage_airports){
        if(user_choice=='1') curr_screen=screen::add_airport;
        else if(user_choice=='2') curr_screen=screen::delete_airport;
        else if(user_choice=='3') curr_screen=screen::airport_list;
        else if(user_choice==(char)ESCAPE) curr_screen=screen::admin_homepage;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::manage_airlines){
        if(user_choice=='1') curr_screen=screen::add_airline;
        else if(user_choice=='2') curr_screen=screen::delete_airline;
        else if(user_choice=='3') curr_screen=screen::airline_list;
        else if(user_choice==(char)ESCAPE) curr_screen=screen::admin_homepage;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::manage_airplane_models){
        if(user_choice=='1') curr_screen=screen::add_airplane_model;
        else if(user_choice=='2') curr_screen=screen::delete_airplane_model;
        else if(user_choice=='3') curr_screen=screen::airplane_model_list;
        else if(user_choice==(char)ESCAPE) curr_screen=screen::admin_homepage;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::manage_routes){
        if(user_choice=='1') curr_screen=screen::add_route;
        else if(user_choice=='2') curr_screen=screen::delete_route;
        else if(user_choice=='3') curr_screen=screen::route_list;
        else if(user_choice==(char)ESCAPE) curr_screen=screen::admin_homepage;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::manage_airplanes){
        if(user_choice=='1') curr_screen=screen::add_airplane;
        else if(user_choice=='2') curr_screen=screen::delete_airplane;
        else if(user_choice=='3') curr_screen=screen::airplane_list;
        else if(user_choice==(char)ESCAPE) curr_screen=screen::admin_homepage;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::manage_imp_updates){
        if(user_choice=='1') curr_screen=screen::add_imp_update;
        else if(user_choice=='2') curr_screen=screen::delete_imp_update;
        else if(user_choice=='3') curr_screen=screen::imp_update_list;
        else if(user_choice==(char)ESCAPE) curr_screen=screen::admin_homepage;
        // else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::logged_out){
        curr_screen=screen::usage_type;
    }
    setCursorVisibility(true);
    return NULL;
}