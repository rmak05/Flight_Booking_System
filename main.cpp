// #include "frontend.cpp"
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
    fflush(stdin);
    char ch='a';
    while(true){
        ch=(char)getch();
        convertToLowercase(ch);
        if((ch=='y') || (ch=='n') || (ch=='q')) break;
    }
    return ch;
}

char inputEscape(){
    fflush(stdin);
    char ch='a';
    while(true){
        ch=(char)getch();
        if(ch==((char)ESCAPE)) break;           
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

// return number of lines printed upto the line following the title
int printBasicScreen(int height){
    int num_lines=0;
    num_lines+=printTopBorder();
    num_lines+=printTitle();
    num_lines+=printLine();
    num_lines+=printLine();
    num_lines+=printLine(line::dashed);
    num_lines+=printLine();
    for(int i=0;i<height;i++) printLine();
    printBottomBorder();
    return num_lines;
}

void* manageAirportsScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. Add Airport","2. Delete Airport");
    printLine("3. View Airport List");
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void addAirportScreen(){
    system("cls");
    int line=0;
    char a_name[LARGE_SIZE+1],a_city[MEDIUM_SIZE+1],a_code[SMALL_SIZE+1];
    char yesno;

    line+=printBasicScreen(10);
    line+=takeInputSetCursor("Airport Name : ",a_name,sizeof(a_name),line);
    line+=takeInputSetCursor("Airport City : ",a_city,sizeof(a_city),line);
    line+=takeInputSetCursor("Airport Code : ",a_code,sizeof(a_code),line);
    convertToUppercase(a_code);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::add_airport;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_airports;
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
}

void deleteAirportScreen(){
    system("cls");
    int line=0;
    char a_code[SMALL_SIZE+1];
    char yesno;

    line+=printBasicScreen(7);
    line+=takeInputSetCursor("Airport Code : ",a_code,sizeof(a_code),line);
    convertToUppercase(a_code);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::delete_airport;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_airports;
        return;
    }
    curr_screen=screen::manage_airports;

    code_to_airport.erase(a_code);
    code_to_airport.copy_to_file("airport_data.bin");
    line+=printOutputSetCursor("Airport deleted successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void airportListScreen(){
    curr_screen=screen::manage_airports;
    system("cls");
    int line=0,list_size=0;
    vector<airport*> airport_list;

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    printLine();
    printLine("Press Escape to go back");
    printLine();
    printLine(line::dashed);

    code_to_airport.traverse(airport_list);
    list_size=airport_list.size();
    for(int i=0;i<list_size;i++){
        printLine();
        (*airport_list[i]).display();
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
}

void* manageAirlinesScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. Add Airline","2. Delete Airline");
    printLine("3. View Airline List");
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void addAirlineScreen(){
    system("cls");
    int line=0;
    char a_name[MEDIUM_SIZE+1];
    char yesno;

    line+=printBasicScreen(7);
    line+=takeInputSetCursor("Airline Name : ",a_name,sizeof(a_name),line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::add_airline;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_airlines;
        return;
    }
    curr_screen=screen::manage_airlines;

    airline *_airline;
    fstream airline_file;
    _airline = new airline(a_name);
    airline_file.open("airline_data.bin",ios::out | ios::app | ios::binary);
    airline_file.seekp(0,ios::beg);
    airline_file.write((char*)_airline,sizeof(airline));
    airline_file.close();
    name_to_airline[a_name]=_airline;
    line+=printOutputSetCursor("Airline added successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void deleteAirlineScreen(){
    system("cls");
    int line=0;
    char a_name[MEDIUM_SIZE+1];
    char yesno;

    line+=printBasicScreen(7);
    line+=takeInputSetCursor("Airline Name : ",a_name,sizeof(a_name),line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::delete_airline;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_airlines;
        return;
    }
    curr_screen=screen::manage_airlines;

    name_to_airline.erase(a_name);
    name_to_airline.copy_to_file("airline_data.bin");
    line+=printOutputSetCursor("Airline deleted successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void airlineListScreen(){
    curr_screen=screen::manage_airlines;
    system("cls");
    int line=0,list_size=0;
    vector<airline*> airline_list;

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    printLine();
    printLine("Press Escape to go back");
    printLine();
    printLine(line::dashed);

    name_to_airline.traverse(airline_list);
    list_size=airline_list.size();
    for(int i=0;i<list_size;i++){
        printLine();
        (*airline_list[i]).display();
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
}

void controlCenter(){
    curr_screen=screen::usage_type;
    pthread_t printUserScreen_thread,printUserTypeSelectionScreen_thread,takeInput_thread,printAdminScreen_thread,printLoggedOutScreen_thread,manageAirportsScreen_thread,manageAirlinesScreen_thread;

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
                pthread_create(&manageAirportsScreen_thread,NULL,manageAirportsScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(manageAirportsScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::add_airport :
                addAirportScreen();
                break;
            case screen::delete_airport :
                deleteAirportScreen();
                break;
            case screen::airport_list :
                airportListScreen();
                break;
            case screen::manage_airlines :
                pthread_create(&manageAirlinesScreen_thread,NULL,manageAirlinesScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(manageAirlinesScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::add_airline :
                addAirlineScreen();
                break;
            case screen::delete_airline :
                deleteAirlineScreen();
                break;
            case screen::airline_list :
                airlineListScreen();
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
    controlCenter();
    // if(name_to_airline.find("Fly Emirates")) cout<<"found\n";
    return 0;
}

// check if all input characters are trie characters are not