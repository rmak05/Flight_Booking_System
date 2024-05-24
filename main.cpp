#include <iostream>
#include <cstring>
#include <windows.h>
#include <pthread.h>
#include <conio.h>
#define SCREEN_MAX_WIDTH 135
#define SCREEN_WIDTH 120
#define LEFT_BORDER_WIDTH 7
#define TEXT_PADDING 10
#define IMPORTANT_UPDATES_TEXT_DELAY 250
#define IMP_UPDATES_WIDTH 75
using namespace std;

char user_choice;

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

struct circular_list_text_node{
    char node_value;
    circular_list_text_node *next;

    circular_list_text_node(){
        node_value='\0';
        next=NULL;
    }

    circular_list_text_node(char value){
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

void printLine(const char *s1,const char *s2){
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
void printLine(const char *s,const int a,const int desired_text_padding){
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

circular_list_text_node* makeTextCircular(const char *s,const int imp_updates_width){
    circular_list_text_node *head=NULL,*temp=NULL;
    if(s[0]=='\0') return head;
    head = new circular_list_text_node(' ');
    temp=head;
    for(int i=0;i<imp_updates_width-1;i++){
        temp->next = new circular_list_text_node(' ');
        temp=temp->next;
    }
    for(int i=0;s[i]!='\0';i++){
        temp->next = new circular_list_text_node(s[i]);
        temp=temp->next;
    }
    temp->next=head;
    return head;
}

void* printScreen(void *p){
    int imp_news_line=0;
    system("cls");
    char text[SCREEN_WIDTH];

    // top line
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)201;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)187<<"\n";
    imp_news_line++;

    // title
    strcpy(text,"______ _ _       _     _     ______             _    _                _____           _                 ");
    printLine(text,align::center);
    imp_news_line++;
    strcpy(text,"|  ___| (_)     | |   | |    | ___ \\           | |  (_)              /  ___|         | |                ");
    printLine(text,align::center);
    imp_news_line++;
    strcpy(text,"| |_  | |_  __ _| |__ | |_   | |_/ / ___   ___ | | ___ _ __   __ _   \\ `--. _   _ ___| |_ ___ _ __ ___  ");
    printLine(text,align::center);
    imp_news_line++;
    strcpy(text,"|  _| | | |/ _` | '_ \\| __|  | ___ \\/ _ \\ / _ \\| |/ / | '_ \\ / _` |   `--. \\ | | / __| __/ _ \\ '_ ` _ \\ ");
    printLine(text,align::center);
    imp_news_line++;
    strcpy(text,"| |   | | | (_| | | | | |_   | |_/ / (_) | (_) |   <| | | | | (_| |  /\\__/ / |_| \\__ \\ ||  __/ | | | | |");
    printLine(text,align::center);
    imp_news_line++;
    strcpy(text,"\\_|   |_|_|\\__, |_| |_|\\__|  \\____/ \\___/ \\___/|_|\\_\\_|_| |_|\\__, |  \\____/ \\__, |___/\\__\\___|_| |_| |_|");
    printLine(text,align::center);
    imp_news_line++;
    strcpy(text,"            __/ |                                             __/ |          __/ |                      ");
    printLine(text,align::center);
    imp_news_line++;
    strcpy(text,"           |___/                                             |___/          |___/                       "); 
    printLine(text,align::center);
    imp_news_line++;

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
    
    printLine("1. Book Ticket","2. Cancel Ticket");
    printLine();
    printLine();

    // bottom line
    for(int i=0;i<LEFT_BORDER_WIDTH;i++) cout<<" ";
    cout<<(char)200;
    for(int i=0;i<SCREEN_WIDTH-2;i++) cout<<(char)205;
    cout<<(char)188<<"\n";

    // important updates
    HANDLE imp_updates_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD imp_updates_coord;
    imp_updates_coord.X=LEFT_BORDER_WIDTH+1+desired_left_space+imp_updates_title_text_size;
    imp_updates_coord.Y=imp_news_line;
    circular_list_text_node *imp_text_ll=makeTextCircular(imp_text,IMP_UPDATES_WIDTH),*temp_ll;
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
        if(user_choice=='x' ||user_choice=='X'){
            break;
        }
    }

    return NULL;
}

void* takeInput(void*){
    while(true){
        user_choice=(char)getch();
        if(user_choice=='x' ||user_choice=='X') break;
    }
    return NULL;
}

void initiate(){
    pthread_t printScreen_thread,input_thread;
    pthread_create(&printScreen_thread,NULL,printScreen,NULL);
    pthread_create(&input_thread,NULL,takeInput,NULL);
    pthread_join(printScreen_thread,NULL);
    pthread_join(input_thread,NULL);
}

int main(){
    initiate();
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