#include "frontend.cpp"
#include "classes.cpp"

void convertToLowercase(char& ch){
    if(('A'<=ch) && (ch<='Z')){
        ch=(char)(ch-'A'+'a');
    }
}

void convertToLowercase(char *s){
    int n=strlen(s);
    for(int i=0;i<n;i++){
        convertToLowercase(s[i]);
    }
}

void convertToUppercase(char& ch){
    if(('a'<=ch) && (ch<='z')){
        ch=(char)(ch-'a'+'A');
    }
}

void convertToUppercase(char *s){
    int n=strlen(s);
    for(int i=0;i<n;i++){
        convertToUppercase(s[i]);
    }
}

int getInputColumnCoordinate(const char *text){
    int text_size=strlen(text),column;
    column=LEFT_BORDER_WIDTH+1+TEXT_PADDING+text_size;    // zero based indexing for column
    return column;
}

char inputYesNo(){
    char ch='a';
    while(true){
        ch=(char)getch();
        convertToLowercase(ch);
        if((ch=='y') || (ch=='n')) break;
    }
    return ch;
}

// returns number of lines printed
int takeInputSetCursor(const char *prompt, char *input_string, int input_size, int row){
    int num_lines=0;
    HANDLE input_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD input_coord;
    BOOL handle_success;

    fflush(stdin);
    input_coord.X=0;
    input_coord.Y=row;
    handle_success=SetConsoleCursorPosition(input_handle,input_coord);
    if(!handle_success) return (-1);
    printLine(prompt);
    num_lines++;
    input_coord.X=getInputColumnCoordinate(prompt);
    handle_success=SetConsoleCursorPosition(input_handle,input_coord);
    if(!handle_success) return (-1);
    customInput(input_string,input_size);
    return num_lines;
}

// returns number of lines printed
int takeInputSetCursor(const char *prompt, char& input_char, int row){
    int num_lines=0;
    HANDLE input_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD input_coord;
    BOOL handle_success;

    fflush(stdin);
    input_coord.X=0;
    input_coord.Y=row;
    handle_success=SetConsoleCursorPosition(input_handle,input_coord);
    if(!handle_success) return (-1);
    printLine(prompt);
    num_lines++;
    input_coord.X=getInputColumnCoordinate(prompt);
    handle_success=SetConsoleCursorPosition(input_handle,input_coord);
    if(!handle_success) return (-1);
    input_char=inputYesNo();
    return num_lines;
}

// returns number of lines printed
int printOutputSetCursor(const char *prompt, int row){
    int num_lines=0;
    HANDLE output_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD output_coord;
    BOOL handle_success;

    output_coord.X=0;
    output_coord.Y=row;
    handle_success=SetConsoleCursorPosition(output_handle,output_coord);
    if(!handle_success) return (-1);
    printLine(prompt);
    num_lines++;
    return num_lines;
}

void addAirportScreen(){
    system("cls");
    int line=0;
    char a_name[LARGE_SIZE+1],a_city[MEDIUM_SIZE+1],a_code[SMALL_SIZE+1];
    char yesno;

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    line+=printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printLine();
    printBottomBorder();

    line+=takeInputSetCursor("Airport Name : ",a_name,sizeof(a_name),line);
    line+=takeInputSetCursor("Airport City : ",a_city,sizeof(a_city),line);
    line+=takeInputSetCursor("Airport Code : ",a_code,sizeof(a_code),line);
    convertToUppercase(a_code);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES and 'N' for NO : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::add_airport;
        return;
    }
    curr_screen=screen::manage_airports;

    airport *_airport;
    fstream airport_file;
    _airport = new airport(a_name,a_city,a_code);
    airport_file.open("airport_data.bin",ios::out | ios::app | ios::binary);
    airport_file.seekp(0,ios::beg);
    airport_file.write((char*)_airport,sizeof(airport));
    airport_file.close();
    code_to_airport[a_code]=_airport;
    line+=printOutputSetCursor("Airport added successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();

    cout<<"\n\n\n\n\n\n";
}

void controlCenter(){
    curr_screen=screen::usage_type;
    pthread_t printUserScreen_thread,printUserTypeSelectionScreen_thread,takeInput_thread,printAdminScreen_thread,printLoggedOutScreen_thread,printManageAirportsScreen_thread;

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
            case screen::manage_airports :
                pthread_create(&printManageAirportsScreen_thread,NULL,printManageAirportsScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(printManageAirportsScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::add_airport :
                addAirportScreen();
                break;
            case screen::program_exit :
                break;
            default :
                break;
        }
    }
}

int main(){
    initializeDataFromFiles();
    // controlCenter();
    return 0;
}