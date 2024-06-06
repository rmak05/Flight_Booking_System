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
int takeInputSetCursor(const char *prompt, int& input_num, int row){
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
    cin>>input_num;
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

void* manageAirplaneModelsScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. Add Airplane Model","2. Delete Airplane Model");
    printLine("3. View Airplane Model List");
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void addAirplaneModelScreen(){
    system("cls");
    int line=0,capacity;
    char a_name[MEDIUM_SIZE+1];
    char yesno;

    line+=printBasicScreen(8);
    line+=takeInputSetCursor("Airplane Model Name : ",a_name,sizeof(a_name),line);
    line+=takeInputSetCursor("Passenger Capacity  : ",capacity,line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::add_airplane_model;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_airplane_models;
        return;
    }
    curr_screen=screen::manage_airplane_models;

    airplane_model *_airplane_model;
    fstream airplane_model_file;
    _airplane_model = new airplane_model(capacity,a_name);
    airplane_model_file.open("airplane_model_data.bin",ios::out | ios::app | ios::binary);
    airplane_model_file.seekp(0,ios::beg);
    airplane_model_file.write((char*)_airplane_model,sizeof(airplane_model));
    airplane_model_file.close();
    name_to_airplane_model[a_name]=_airplane_model;

    line+=printOutputSetCursor("Airplane Model added successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void deleteAirplaneModelScreen(){
    system("cls");
    int line=0;
    char a_name[MEDIUM_SIZE+1];
    char yesno;

    line+=printBasicScreen(7);
    line+=takeInputSetCursor("Airplane Model Name : ",a_name,sizeof(a_name),line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::delete_airplane_model;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_airplane_models;
        return;
    }
    curr_screen=screen::manage_airplane_models;

    name_to_airplane_model.erase(a_name);
    name_to_airplane_model.copy_to_file("airplane_model_data.bin");
    line+=printOutputSetCursor("Airplane Model deleted successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void airplaneModelListScreen(){
    curr_screen=screen::manage_airplane_models;
    system("cls");
    int line=0,list_size=0;
    vector<airplane_model*> airplane_model_list;

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    printLine();
    printLine("Press Escape to go back");
    printLine();
    printLine(line::dashed);

    name_to_airplane_model.traverse(airplane_model_list);
    list_size=airplane_model_list.size();
    for(int i=0;i<list_size;i++){
        printLine();
        (*airplane_model_list[i]).display();
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
}

void* manageRoutesScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. Add Route","2. Delete Route");
    printLine("3. View Route List");
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void addRouteScreen(){
    system("cls");
    int line=0,r_distance;
    char s_airport[SMALL_SIZE+1],d_airport[SMALL_SIZE+1],r_code[2*SMALL_SIZE+1];
    char yesno;

    line+=printBasicScreen(9);
    line+=takeInputSetCursor("Starting Airport Code    : ",s_airport,sizeof(s_airport),line);
    line+=takeInputSetCursor("Destination Airport Code : ",d_airport,sizeof(d_airport),line);
    convertToUppercase(s_airport);
    convertToUppercase(d_airport);
    strcpy(r_code,s_airport);
    strcpy(r_code+SMALL_SIZE,d_airport);
    line+=takeInputSetCursor("Route Distance           : ",r_distance,line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::add_route;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_routes;
        return;
    }
    curr_screen=screen::manage_routes;

    route *_route;
    fstream route_file;
    _route = new route(r_distance,s_airport,d_airport);
    route_file.open("route_data.bin",ios::out | ios::app | ios::binary);
    route_file.seekp(0,ios::beg);
    route_file.write((char*)_route,sizeof(route));
    route_file.close();
    code_to_route[r_code]=_route;

    line+=printOutputSetCursor("Route added successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void deleteRouteScreen(){
    system("cls");
    int line=0;
    char r_code[2*SMALL_SIZE+1];
    char yesno;

    line+=printBasicScreen(7);
    line+=takeInputSetCursor("Route Code : ",r_code,sizeof(r_code),line);
    convertToUppercase(r_code);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::delete_route;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_routes;
        return;
    }
    curr_screen=screen::manage_routes;

    code_to_route.erase(r_code);
    code_to_route.copy_to_file("route_data.bin");
    line+=printOutputSetCursor("Route deleted successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void routeListScreen(){
    curr_screen=screen::manage_routes;
    system("cls");
    int line=0,list_size=0;
    vector<route*> route_list;

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    printLine();
    printLine("Press Escape to go back");
    printLine();
    printLine(line::dashed);

    code_to_route.traverse(route_list);
    list_size=route_list.size();
    for(int i=0;i<list_size;i++){
        printLine();
        (*route_list[i]).display();
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
}

void* manageAirplanesScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. Add Airplane","2. Delete Airplane");
    printLine("3. View Airplane List");
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void addAirplaneScreen(){
    system("cls");
    int line=0,cost,d_time,a_time;
    char _airline_name[MEDIUM_SIZE+1],_model_name[MEDIUM_SIZE+1],_route_code[2*SMALL_SIZE+1],s_airport[SMALL_SIZE+1];
    char yesno;

    line+=printBasicScreen(12);
    line+=takeInputSetCursor("Airline Name        : ",_airline_name,sizeof(_airline_name),line);
    line+=takeInputSetCursor("Airplane Model Name : ",_model_name,sizeof(_model_name),line);
    line+=takeInputSetCursor("Route Code          : ",_route_code,sizeof(_route_code),line);
    convertToUppercase(_route_code);
    for(int i=0;i<SMALL_SIZE;i++) s_airport[i]=_route_code[i];
    s_airport[SMALL_SIZE]='\0';
    line+=takeInputSetCursor("Departure Time      : ",d_time,line);
    line+=takeInputSetCursor("Arrival Time        : ",a_time,line);
    line+=takeInputSetCursor("Ticket Cost         : ",cost,line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::add_airplane;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_airplanes;
        return;
    }
    curr_screen=screen::manage_airplanes;

    airline *_airline=name_to_airline[_airline_name];
    airplane_model *_airplane_model=name_to_airplane_model[_model_name];
    route *_route=code_to_route[_route_code];
    airplane *_airplane;
    fstream airplane_file;
    _airplane = new airplane((*_airline),(*_airplane_model),(*_route),cost,d_time,a_time);
    airplane_file.open("airplane_data.bin",ios::out | ios::app | ios::binary);
    airplane_file.seekp(0,ios::beg);
    airplane_file.write((char*)_airplane,sizeof(airplane));
    airplane_file.close();
    // (*_airplane).add_to_airport();
    airport *_airport;
    _airport=code_to_airport[s_airport];
    if(_airport!=NULL) (*_airport).add_airplane(_airplane);
    line+=printOutputSetCursor("Airplane added successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void deleteAirplaneScreen(){
    system("cls");
    int line=0,d_time;
    char _airline_name[MEDIUM_SIZE+1],_route_code[2*SMALL_SIZE+1],s_airport[SMALL_SIZE+1];
    char yesno;

    line+=printBasicScreen(10);
    line+=takeInputSetCursor("Airline Name   : ",_airline_name,sizeof(_airline_name),line);
    line+=takeInputSetCursor("Route Code     : ",_route_code,sizeof(_route_code),line);
    convertToUppercase(_route_code);
    for(int i=0;i<SMALL_SIZE;i++) s_airport[i]=_route_code[i];
    s_airport[SMALL_SIZE]='\0';
    line+=takeInputSetCursor("Departure Time : ",d_time,line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::delete_airplane;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_airplanes;
        return;
    }
    curr_screen=screen::manage_airplanes;

    airport *_airport;
    _airport=code_to_airport[s_airport];
    if(_airport!=NULL) (*_airport).delete_airplane(_airline_name,_route_code,d_time);
    vector<airport*> airport_list;
    int list_size;
    code_to_airport.traverse(airport_list);
    list_size=airport_list.size();
    fstream airplane_file;
    airplane_file.open("airplane_data.bin",ios::out);
    airplane_file.close();
    for(int i=0;i<list_size;i++) (*airport_list[i]).copy_flights_to_file("airplane_data.bin");

    line+=printOutputSetCursor("Airplane deleted successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void airplaneListScreen(){
    curr_screen=screen::manage_airplanes;
    system("cls");
    int line=0,list_size=0,size=0;
    vector<airport*> airport_list;
    vector<airplane*> airplane_list;

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
        airplane_list=(*airport_list[i]).get_outgoing_flights();
        size=airplane_list.size();
        for(int j=0;j<size;j++){
            printLine();
            (*airplane_list[j]).display();
            if(!(i==list_size-1 && j==size-1)){
                printLine();
                printLine(line::dotted);
            }
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
}

void controlCenter(){
    curr_screen=screen::usage_type;
    pthread_t printUserScreen_thread,printUserTypeSelectionScreen_thread,takeInput_thread,printAdminScreen_thread,printLoggedOutScreen_thread,manageAirportsScreen_thread,manageAirlinesScreen_thread,manageAirplaneModelsScreen_thread,manageRoutesScreen_thread,manageAirplanesScreen_thread;

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
            case screen::manage_airplane_models :
                pthread_create(&manageAirplaneModelsScreen_thread,NULL,manageAirplaneModelsScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(manageAirplaneModelsScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::add_airplane_model :
                addAirplaneModelScreen();
                break;
            case screen::delete_airplane_model :
                deleteAirplaneModelScreen();
                break;
            case screen::airplane_model_list :
                airplaneModelListScreen();
                break;
            case screen::manage_routes :
                pthread_create(&manageRoutesScreen_thread,NULL,manageRoutesScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(manageRoutesScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::add_route :
                addRouteScreen();
                break;
            case screen::delete_route :
                deleteRouteScreen();
                break;
            case screen::route_list :
                routeListScreen();
                break;
            case screen::manage_airplanes :
                pthread_create(&manageAirplanesScreen_thread,NULL,manageAirplanesScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(manageAirplanesScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::add_airplane :
                addAirplaneScreen();
                break;
            case screen::delete_airplane :
                deleteAirplaneScreen();
                break;
            case screen::airplane_list :
                airplaneListScreen();
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