#include <iostream>
#include <cstring>
#include <windows.h>
#include <pthread.h>
#include <conio.h>
#include "classes.cpp"
#define SCREEN_MAX_WIDTH 135
#define SCREEN_WIDTH 120
#define LEFT_BORDER_WIDTH 7
#define TEXT_PADDING 10
#define IMPORTANT_UPDATES_TEXT_DELAY 250
#define IMP_UPDATES_WIDTH 75
using namespace std;

unsigned char user_choice;
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

// enum user_type{
//     user  = 0,
//     admin = 1,
//     guest = 2
// };

enum screen{
    usage_type     = 0,
    user_homepage  = 1,
    admin_homepage = 2,
    program_exit   = 3,
    logged_out     = 4 
};

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

void printLine(){
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)186;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<" ";
    cout<<(char)186<<"\n";
}

void printLine(const int a){
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
    cout<<(char)186;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<line_ch;
    cout<<(char)186<<"\n";
}

void printLine(const char *s){
    int n=strlen(s);
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)186;
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    cout<<s;
    for(int i=0;i<SCREEN_WIDTH-TEXT_PADDING-2-n;i++) cout<<" ";
    cout<<(char)186<<"\n";
}

void printLine(const char *s1, const char *s2){
    int n1=strlen(s1),n2=strlen(s2);
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)186;
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    cout<<s1;
    for(int i=0;i<(SCREEN_WIDTH-2)/2-TEXT_PADDING-n1;i++) cout<<" ";
    for(int i=0;i<TEXT_PADDING;i++) cout<<" ";
    cout<<s2;
    for(int i=0;i<(SCREEN_WIDTH-2)/2-TEXT_PADDING-n2;i++) cout<<" ";
    cout<<(char)186<<"\n";
}

void printLine(const char *s, const int a){
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
            left_space=(SCREEN_WIDTH-2-n)/2;
            right_space=SCREEN_WIDTH-2-n-left_space;
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

// desired_text_padding will by default be taken as left padding unless it is for align::right
// desired_text_padding is ignored for align::center
void printLine(const char *s, const int a, const int desired_text_padding){
    int n=strlen(s),left_space=1,right_space=1;
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
    cout<<(char)186;
    for(int i=0;i<left_space;i++) cout<<" ";
    cout<<s;
    for(int i=0;i<right_space;i++) cout<<" ";
    cout<<(char)186<<"\n";
}

circularListTextNode* makeTextCircular(const char *s, const int imp_updates_width){
    circularListTextNode *head=NULL,*temp=NULL;
    if(s[0]=='\0') return head;
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
    strcpy(text,"______ _ _       _     _     ______             _    _                _____           _                 ");
    printLine(text,align::center);
    num_lines++;
    strcpy(text,"|  ___| (_)     | |   | |    | ___ \\           | |  (_)              /  ___|         | |                ");
    printLine(text,align::center);
    num_lines++;
    strcpy(text,"| |_  | |_  __ _| |__ | |_   | |_/ / ___   ___ | | ___ _ __   __ _   \\ `--. _   _ ___| |_ ___ _ __ ___  ");
    printLine(text,align::center);
    num_lines++;
    strcpy(text,"|  _| | | |/ _` | '_ \\| __|  | ___ \\/ _ \\ / _ \\| |/ / | '_ \\ / _` |   `--. \\ | | / __| __/ _ \\ '_ ` _ \\ ");
    printLine(text,align::center);
    num_lines++;
    strcpy(text,"| |   | | | (_| | | | | |_   | |_/ / (_) | (_) |   <| | | | | (_| |  /\\__/ / |_| \\__ \\ ||  __/ | | | | |");
    printLine(text,align::center);
    num_lines++;
    strcpy(text,"\\_|   |_|_|\\__, |_| |_|\\__|  \\____/ \\___/ \\___/|_|\\_\\_|_| |_|\\__, |  \\____/ \\__, |___/\\__\\___|_| |_| |_|");
    printLine(text,align::center);
    num_lines++;
    strcpy(text,"            __/ |                                             __/ |          __/ |                      ");
    printLine(text,align::center);
    num_lines++;
    strcpy(text,"           |___/                                             |___/          |___/                       "); 
    printLine(text,align::center);
    num_lines++;
    return num_lines;
}

// returns number of lines printed
int printTopBorder(){
    int num_lines=0;
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)201;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)187<<"\n";
    num_lines++;
    return num_lines;
}

// returns number of lines printed
int printBottomBorder(){
    int num_lines=0;
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)200;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)188<<"\n";
    num_lines++;
    return num_lines;
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
    char imp_text[]="The flight to Bhubaneswar has been delayed. Please wait for further updates.  |  The flight to Pune will arrive at 3:00 PM.  |  The flight to Mumbai has been cancelled.";
    int imp_updates_title_text_size=strlen(imp_updates_title_text);
    int desired_left_space=TEXT_PADDING;
    // int desired_left_space=(SCREEN_WIDTH-2-imp_updates_title_text_size-IMP_UPDATES_WIDTH)/2;
    printLine(imp_updates_title_text,align::custom,desired_left_space);      

    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. Open profile","2. Book ticket");
    printLine("3. Cancel ticket","4. Check flight availability");
    printLine("5. Log out");
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
            cout<<(temp_ll->node_value);
            temp_ll=temp_ll->next;
        }
        imp_text_ll=imp_text_ll->next;
        cout<<"\n\n\n\n\n\n\n\n\n\n\n";
        Sleep(IMPORTANT_UPDATES_TEXT_DELAY);
        if(user_choice!='\0') break;
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
    char imp_text[]="The flight to Bhubaneswar has been delayed. Please wait for further updates.  |  The flight to Pune will arrive at 3:00 PM.  |  The flight to Mumbai has been cancelled.";
    int imp_updates_title_text_size=strlen(imp_updates_title_text);
    int desired_left_space=TEXT_PADDING;
    // int desired_left_space=(SCREEN_WIDTH-2-imp_updates_title_text_size-IMP_UPDATES_WIDTH)/2;
    printLine(imp_updates_title_text,align::custom,desired_left_space);      

    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. Manage Important Updates","2. Manage Airports");
    printLine("3. Manage Airlines","4. Manage Airplane Models");
    printLine("5. Manage Routes","6. Manage Flights");
    printLine("7. Log out");
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
            cout<<(temp_ll->node_value);
            temp_ll=temp_ll->next;
        }
        imp_text_ll=imp_text_ll->next;
        cout<<"\n\n\n\n\n\n\n\n\n\n\n";
        Sleep(IMPORTANT_UPDATES_TEXT_DELAY);
        if(user_choice!='\0') break;
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
    printLine("1. Guest",align::center);
    printLine("2. User",align::center);
    printLine("3. Admin",align::center);
    printLine();
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
    printLine("Successfully Logged Out",align::center);
    printLine("Press any key to continue ...",align::center);
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void* takeInput(void *p){
    user_choice=(unsigned char)getch();
    if(curr_screen==screen::usage_type){
        if(user_choice=='1'){
            
        }
        else if(user_choice=='2') curr_screen=screen::user_homepage;
        else if(user_choice=='3') curr_screen=screen::admin_homepage;
        else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::user_homepage){
        if(user_choice=='1'){
            
        }
        else if(user_choice=='5') curr_screen=screen::logged_out;
        else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::admin_homepage){
        if(user_choice=='1'){
            
        }
        else if(user_choice=='7') curr_screen=screen::logged_out;
        else if(user_choice=='x') curr_screen=screen::program_exit;
    }
    else if(curr_screen==screen::logged_out){
        curr_screen=screen::usage_type;
    }
    return NULL;
}

void controlCenter(){
    curr_screen=screen::usage_type;
    pthread_t printUserScreen_thread,printUserTypeSelectionScreen_thread,takeInput_thread,printAdminScreen_thread,printLoggedOutScreen_thread;

    while(true){
        if(curr_screen==program_exit) break;
        switch(curr_screen){
            case screen::usage_type :
                pthread_create(&printUserTypeSelectionScreen_thread,NULL,printUserTypeSelectionScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(printUserTypeSelectionScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::user_homepage :
                pthread_create(&printUserScreen_thread,NULL,printUserScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(printUserScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::admin_homepage :
                pthread_create(&printAdminScreen_thread,NULL,printAdminScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(printAdminScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::logged_out :
                pthread_create(&printLoggedOutScreen_thread,NULL,printLoggedOutScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(printLoggedOutScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::program_exit :
                break;
            default :
                break;
        }
    }
}

int main(){
    controlCenter();
    return 0;
}

// ascii art
// big
// doom (currently using)
// standard
// printLine() should return number of lines printed