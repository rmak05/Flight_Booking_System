#include "classes.cpp"

// x=column , y=row
bool setCursorPosition(int x, int y){
    HANDLE _handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD _coord;
    BOOL _success;
    _coord.X=x;
    _coord.Y=y;
    _success=SetConsoleCursorPosition(_handle,_coord);
    return _success;
}

// point.first=x=column , point.second=y=row
bool setCursorPosition(pair<int,int>& point){
    HANDLE _handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD _coord;
    BOOL _success;
    _coord.X=point.first;
    _coord.Y=point.second;
    _success=SetConsoleCursorPosition(_handle,_coord);
    return _success;
}

// point.first=x=column , point.second=y=row
bool getCursorPosition(pair<int,int>& point){
    CONSOLE_SCREEN_BUFFER_INFO buff_info;
    HANDLE _handle=GetStdHandle(STD_OUTPUT_HANDLE);
    BOOL _success=GetConsoleScreenBufferInfo(_handle,&buff_info);
    point.first=buff_info.dwCursorPosition.X;
    point.second=buff_info.dwCursorPosition.Y;
    return _success;
}

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
    printLine(prompt,colour::green);
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
    printLine(prompt,colour::green);
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
    printLine(prompt,colour::green);
    num_lines++;
    input_coord.X=getInputColumnCoordinate(prompt);
    handle_success=SetConsoleCursorPosition(input_handle,input_coord);
    if(!handle_success) return (-1);
    setBackgroundColour();
    cin>>input_num;
    resetBackgorund();
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
    printLine(prompt,colour::green);
    num_lines++;
    return num_lines;
}

// returns number of lines printed
int printOutputSetCursor(const char *prompt, align alignment, int row){
    int num_lines=0;
    HANDLE output_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD output_coord;
    BOOL handle_success;

    output_coord.X=0;
    output_coord.Y=row;
    handle_success=SetConsoleCursorPosition(output_handle,output_coord);
    if(!handle_success) return (-1);
    printLine(prompt,alignment,colour::green);
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
    printLine("1. ","Add Airport","2. ","Delete Airport",colour::green,true);
    printLine("3. ","View Airport List",colour::green,true);
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

    if(code_to_airport.find(a_code)){
        line+=printOutputSetCursor("Airport code already exists.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
    if(strlen(a_code)!=SMALL_SIZE){
        line+=printOutputSetCursor("Invalid airport code.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }

    airport *_airport;
    fstream airport_file;
    _airport = new airport(a_name,a_city,a_code);
    airport_file.open("Data/airport_data.bin",ios::out | ios::app | ios::binary);
    airport_file.seekp(0,ios::end);
    airport_file.write((char*)_airport,sizeof(airport));
    airport_file.close();
    code_to_airport[a_code]=_airport;

    line+=printOutputSetCursor("Airport added successfully.",line);
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
    code_to_airport.copy_to_file("Data/airport_data.bin");
    line+=printOutputSetCursor("Airport deleted successfully.",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void airportListScreen(){
    setCursorVisibility(false);
    // curr_screen=screen::manage_airports;
    if(user_type=='G') curr_screen=screen::guest_homepage;
    else if(user_type=='U') curr_screen=screen::user_homepage;
    else if(user_type=='A') curr_screen=screen::manage_airports;
    system("cls");
    int line=0,list_size=0;
    vector<airport*> airport_list;

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    printLine();
    printLine("Press Escape to go back",colour::red);
    printLine();
    printLine(line::dashed);

    code_to_airport.traverse(airport_list);
    list_size=airport_list.size();
    for(int i=0;i<list_size;i++){
        printLine();
        (airport_list[i])->display();
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
    setCursorVisibility(true);
}

void* manageAirlinesScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","Add Airline","2. ","Delete Airline",colour::green,true);
    printLine("3. ","View Airline List",colour::green,true);
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

    if(name_to_airline.find(a_name)){
        line+=printOutputSetCursor("Airline already exists.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }

    airline *_airline;
    fstream airline_file;
    _airline = new airline(a_name);
    airline_file.open("Data/airline_data.bin",ios::out | ios::app | ios::binary);
    airline_file.seekp(0,ios::end);
    airline_file.write((char*)_airline,sizeof(airline));
    airline_file.close();
    name_to_airline[a_name]=_airline;

    line+=printOutputSetCursor("Airline added successfully.",line);
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
    name_to_airline.copy_to_file("Data/airline_data.bin");
    line+=printOutputSetCursor("Airline deleted successfully.",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void airlineListScreen(){
    setCursorVisibility(false);
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
    printLine("Press Escape to go back",colour::red);
    printLine();
    printLine(line::dashed);

    name_to_airline.traverse(airline_list);
    list_size=airline_list.size();
    for(int i=0;i<list_size;i++){
        printLine();
        (airline_list[i])->display();
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
    setCursorVisibility(true);
}

void* manageAirplaneModelsScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","Add Airplane Model","2. ","Delete Airplane Model",colour::green,true);
    printLine("3. ","View Airplane Model List",colour::green,true);
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

    if(name_to_airplane_model.find(a_name)){
        line+=printOutputSetCursor("Airplane Model already exists.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }

    airplane_model *_airplane_model;
    fstream airplane_model_file;
    _airplane_model = new airplane_model(capacity,a_name);
    airplane_model_file.open("Data/airplane_model_data.bin",ios::out | ios::app | ios::binary);
    airplane_model_file.seekp(0,ios::end);
    airplane_model_file.write((char*)_airplane_model,sizeof(airplane_model));
    airplane_model_file.close();
    name_to_airplane_model[a_name]=_airplane_model;

    line+=printOutputSetCursor("Airplane Model added successfully.",line);
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
    name_to_airplane_model.copy_to_file("Data/airplane_model_data.bin");
    line+=printOutputSetCursor("Airplane Model deleted successfully.",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void airplaneModelListScreen(){
    setCursorVisibility(false);
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
    printLine("Press Escape to go back",colour::red);
    printLine();
    printLine(line::dashed);

    name_to_airplane_model.traverse(airplane_model_list);
    list_size=airplane_model_list.size();
    for(int i=0;i<list_size;i++){
        printLine();
        (airplane_model_list[i])->display();
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
    setCursorVisibility(true);
}

void* manageRoutesScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","Add Route","2. ","Delete Route",colour::green,true);
    printLine("3. ","View Route List",colour::green,true);
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

    if(!code_to_airport.find(s_airport)){
        line+=printOutputSetCursor("Starting airport doesn't exist.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
    if(!code_to_airport.find(d_airport)){
        line+=printOutputSetCursor("Destination airport doesn't exist.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
    if(strcmp(s_airport,d_airport)==0){
        line+=printOutputSetCursor("Starting and Destination airports are the same.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
    if(code_to_route.find(r_code)){
        line+=printOutputSetCursor("Route code already exists.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }

    route *_route;
    fstream route_file;
    _route = new route(r_distance,s_airport,d_airport);
    route_file.open("Data/route_data.bin",ios::out | ios::app | ios::binary);
    route_file.seekp(0,ios::end);
    route_file.write((char*)_route,sizeof(route));
    code_to_route[r_code]=_route;

    _route = new route(r_distance,d_airport,s_airport);
    route_file.write((char*)_route,sizeof(route));
    strcpy(r_code,_route->get_route_code());
    code_to_route[r_code]=_route;
    route_file.close();

    line+=printOutputSetCursor("Route added successfully.",line);
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
    code_to_route.copy_to_file("Data/route_data.bin");
    line+=printOutputSetCursor("Route deleted successfully.",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void routeListScreen(){
    setCursorVisibility(false);
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
    printLine("Press Escape to go back",colour::red);
    printLine();
    printLine(line::dashed);

    code_to_route.traverse(route_list);
    list_size=route_list.size();
    for(int i=0;i<list_size;i++){
        printLine();
        (route_list[i])->display();
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
    setCursorVisibility(true);
}

void* manageAirplanesScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","Add Airplane","2. ","Delete Airplane",colour::green,true);
    printLine("3. ","View Airplane List",colour::green,true);
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
    if(strlen(_route_code)!=2*SMALL_SIZE){
        curr_screen=screen::add_airplane;
        line+=printOutputSetCursor("Invalid route code.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
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

    if(!name_to_airline.find(_airline_name)){
        line+=printOutputSetCursor("Airline doesn't exist.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
    if(!name_to_airplane_model.find(_model_name)){
        line+=printOutputSetCursor("Airplane Model doesn't exist.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
    if(!code_to_route.find(_route_code)){
        line+=printOutputSetCursor("Route code doesn't exist.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
    airport *_airport;
    _airport=code_to_airport[s_airport];
    if(_airport->find_airplane(_route_code,d_time)){
        line+=printOutputSetCursor("Either flight already exists or there is a clash with another flight.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }

    airline *_airline=name_to_airline[_airline_name];
    airplane_model *_airplane_model=name_to_airplane_model[_model_name];
    route *_route=code_to_route[_route_code];
    airplane *_airplane;
    fstream airplane_file;
    _airplane = new airplane((*_airline),(*_airplane_model),(*_route),cost,d_time,a_time);
    airplane_file.open("Data/airplane_data.bin",ios::out | ios::app | ios::binary);
    airplane_file.seekp(0,ios::end);
    airplane_file.write((char*)_airplane,sizeof(airplane));
    airplane_file.close();
    // (*_airplane).add_to_airport();
    if(_airport!=NULL) (*_airport).add_airplane(_airplane);
    line+=printOutputSetCursor("Airplane added successfully.",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void deleteAirplaneScreen(){
    system("cls");
    int line=0,d_time;
    // char _airline_name[MEDIUM_SIZE+1];
    char _route_code[2*SMALL_SIZE+1],s_airport[SMALL_SIZE+1];
    char yesno;

    line+=printBasicScreen(10);
    // line+=takeInputSetCursor("Airline Name   : ",_airline_name,sizeof(_airline_name),line);
    line+=takeInputSetCursor("Route Code     : ",_route_code,sizeof(_route_code),line);
    convertToUppercase(_route_code);
    if(strlen(_route_code)!=2*SMALL_SIZE){
        curr_screen=screen::delete_airplane;
        line+=printOutputSetCursor("Invalid route code.",line);
        line+=printOutputSetCursor("Press any key to continue ...",line);
        getch();
        return;
    }
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
    if(_airport!=NULL) _airport->delete_airplane(_route_code,d_time);
    vector<airport*> airport_list;
    int list_size;
    code_to_airport.traverse(airport_list);
    list_size=airport_list.size();
    fstream airplane_file;
    airplane_file.open("Data/airplane_data.bin",ios::out);
    airplane_file.close();
    for(int i=0;i<list_size;i++) (airport_list[i])->copy_flights_to_file("Data/airplane_data.bin");

    line+=printOutputSetCursor("Airplane deleted successfully.",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void airplaneListScreen(){
    setCursorVisibility(false);
    curr_screen=screen::manage_airplanes;
    system("cls");
    int line=0,list_size=0,size=0;
    vector<airport*> airport_list;
    vector<airplane*> airplane_list;
    bool first=false;

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    printLine();
    printLine("Press Escape to go back",colour::red);
    printLine();
    printLine(line::dashed);
    printLine();

    code_to_airport.traverse(airport_list);
    list_size=airport_list.size();
    for(int i=0;i<list_size;i++){
        airplane_list=(airport_list[i])->get_outgoing_flights();
        size=airplane_list.size();
        for(int j=0;j<size;j++){
            if(first){
                printLine();
                printLine(line::dotted);
                printLine();
            }
            first=true;
            (airplane_list[j])->display();
        }
    }
    printLine();
    printBottomBorder();
    inputEscape();
    setCursorVisibility(true);
}

void* manageImpUpdatesScreen(void *p){
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("1. ","Add Important Update","2. ","Delete Important Update",colour::green,true);
    printLine("3. ","View Important Update List",colour::green,true);
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    return NULL;
}

void addImpUpdateScreen(){
    system("cls");
    int line=0;
    char _text[2*LARGE_SIZE+1];
    char yesno;

    line+=printBasicScreen(7);
    line+=takeInputSetCursor("Important Update : ",_text,sizeof(_text),line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::add_imp_update;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_imp_updates;
        return;
    }
    curr_screen=screen::manage_imp_updates;

    fstream imp_updates_file;
    imp_updates_file.open("Data/important_updates_data.bin",ios::out | ios::app | ios::binary);
    char *text = new char[2*LARGE_SIZE+1];
    strcpy(text,_text);
    imp_updates_file.write((char*)text,2*LARGE_SIZE+1);
    imp_updates_file.close();

    line+=printOutputSetCursor("Important Update added successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void deleteImpUpdateScreen(){
    system("cls");
    int line=0,serial_num;
    char yesno;

    line+=printBasicScreen(8);
    line+=takeInputSetCursor("Serial Number : ",serial_num,line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        curr_screen=screen::delete_imp_update;
        return;
    }
    else if(yesno=='q'){
        curr_screen=screen::manage_imp_updates;
        return;
    }
    curr_screen=screen::manage_imp_updates;

    if(serial_num<0){
        return;
    }

    int list_size,s_num=0;
    fstream imp_updates_file;
    vector<char*> updates_list;
    char *text = new char[2*LARGE_SIZE+1];
    imp_updates_file.open("Data/important_updates_data.bin",ios::in | ios::app | ios::binary);
    while(imp_updates_file.read((char*)text,2*LARGE_SIZE+1)){
        s_num++;
        if(s_num==serial_num) continue;
        updates_list.push_back(text);
        text = new char[2*LARGE_SIZE+1];
    }
    imp_updates_file.close();
    imp_updates_file.open("Data/important_updates_data.bin",ios::out);
    list_size=updates_list.size();
    for(int i=0;i<list_size;i++) imp_updates_file.write((char*)updates_list[i],2*LARGE_SIZE+1);

    line+=printOutputSetCursor("Important Update deleted successfully",line);
    line+=printOutputSetCursor("Press any key to continue ...",line);
    getch();
}

void impUpdateListScreen(){
    setCursorVisibility(false);
    curr_screen=screen::manage_imp_updates;
    system("cls");
    int line=0,serial_num=0,list_size=0,i=(-1);

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    printLine();
    printLine("Press Escape to go back",colour::red);
    printLine();
    printLine(line::dashed);

    fstream imp_updates_file;
    char *text = new char[2*LARGE_SIZE+1];
    char _text[2*LARGE_SIZE+1+10];
    imp_updates_file.open("Data/important_updates_data.bin",ios::in | ios::binary);
    imp_updates_file.seekg(0,ios::beg);
    while(imp_updates_file.read((char*)text,2*LARGE_SIZE+1)){
        list_size++;
    }
    imp_updates_file.close();
    imp_updates_file.open("Data/important_updates_data.bin",ios::in | ios::binary);
    imp_updates_file.seekg(0,ios::beg);
    while(imp_updates_file.read((char*)text,2*LARGE_SIZE+1)){
        i++;
        serial_num++;
        printLine();
        // sprintf(_text,"%2d. %s",serial_num,text);
        // printLine(_text,colour::_default);
        sprintf(_text,"%2d. ",serial_num);
        printLine(_text,text,colour::green,true);
        if(i!=list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    imp_updates_file.close();

    printLine();
    printBottomBorder();
    inputEscape();
    setCursorVisibility(true);
}

void bookingSuccessful(){
    setCursorVisibility(false);
    curr_screen=screen::user_homepage;
    system("cls");
    printTopBorder();
    printTitle();
    printLine();
    printLine();
    printLine(line::dashed);
    printLine();
    printLine("Ticket booked successfully!",align::center,colour::green);
    printLine("Booking ID : 12345678910",align::center,colour::green);
    printLine("Press any key to continue ...",align::center,colour::green);
    printLine();
    printLine();
    printLine();
    printBottomBorder();
    getch();
    setCursorVisibility(true);
}

// ascii art taken from asciiart.eu
void airplaneAnimation(){
    system("cls");
    setCursorVisibility(false);
    int line=0,temp_line=0;
    line+=printBasicScreen(10);

    int animation_width,animation_height=8,count=0,index,total_width=SCREEN_WIDTH-2-2*TEXT_PADDING;
    animation_width=strlen("        ______                                          ");
    char text[animation_height][animation_width];

    strcpy(text[0],"        ______                                          ");
    strcpy(text[1],"        _\\ _~-\\___                                      ");
    strcpy(text[2],"=  = ==(____AA____D                                     ");
    strcpy(text[3],"            \\_____\\___________________,-~~~~~~~`-.._    ");
    strcpy(text[4],"            /     o O o o o o O O o o o o o o O o  |\\_  ");
    strcpy(text[5],"            `~-.__        ___..----..                  )");
    strcpy(text[6],"                  `---~~\\___________/------------`````  ");
    strcpy(text[7],"                  =  ===(_________D                     ");

    setBackgroundColour();
    setForegroundColour(255,165,0);
    for(int i=animation_width;i>=(2-SCREEN_WIDTH);i--){
        temp_line=line;
        if((i>=animation_width) || (i<=(2-SCREEN_WIDTH))){
            for(int j=0;j<8;j++){
                setCursorPosition(LEFT_BORDER_WIDTH+1,temp_line);
                temp_line++;
                for(int k=0;k<SCREEN_WIDTH-2;k++) cout<<" ";
                cout<<"\n";
            }
        }
        else if(i>=0){
            for(int j=0;j<animation_height;j++){
                setCursorPosition(LEFT_BORDER_WIDTH+1,temp_line);
                temp_line++;
                count=0;
                index=i;
                for(int k=0;index<animation_width && count<SCREEN_WIDTH-2;k++){
                    cout<<text[j][index++];
                    count++;
                }
                while(count<SCREEN_WIDTH-2){
                    cout<<" ";
                    count++;
                }
                cout<<"\n";
            }
        }
        else{
            for(int j=0;j<animation_height;j++){
                setCursorPosition(LEFT_BORDER_WIDTH+1,temp_line);
                temp_line++;
                count=0;
                index=0;
                while(count<(-i)){
                    cout<<" ";
                    count++;
                }
                for(int k=0;k<animation_width && count<SCREEN_WIDTH-2;k++){
                    cout<<text[j][index++];
                    count++;
                }
                while(count<SCREEN_WIDTH-2){
                    cout<<" ";
                    count++;
                }
                cout<<"\n";
            }
        }
        // Sleep(10);
    }
    resetBackgorund();
    setForegroundColour(colour::_default);
    setCursorVisibility(true);
    bookingSuccessful();
}
// add random seat number
// create booking id using seed or time
void printTicket(ticket_details& _details){
    airport *starting_airport,*destination_airport;
    destination_airport=code_to_airport[_details.d_airport];
    starting_airport=code_to_airport[_details.s_airport];

    int d_time[4],a_time[4];
    int d_time_int,a_time_int;
    d_time_int=_details._airplane.get_departure_time();
    a_time_int=_details._airplane.get_arrival_time();
    for(int i=3;i>=0;i--){
        d_time[i]=d_time_int%10;
        a_time[i]=a_time_int%10;
        d_time_int/=10;
        a_time_int/=10;
    }
    double ticket_cost,gst,total_cost;
    total_cost=(double)_details._airplane.get_cost();
    ticket_cost=(20.0*total_cost)/21;
    gst=total_cost-ticket_cost;

    fstream ticket_file;
    ticket_file.open("Ticket/Flight_Ticket.html",ios::out);

    ticket_file<<"<!DOCTYPE html>\n";

    ticket_file<<"<html lang=\"en\">\n";

    ticket_file<<"<head>\n";

    ticket_file<<"\t<meta charset=\"UTF-8\">\n";
    ticket_file<<"\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ticket_file<<"\t<title>Flight Ticket</title>\n";
    ticket_file<<"\t<link rel=\"stylesheet\" href=\"style.css\">\n";

    ticket_file<<"</head>\n";

    ticket_file<<"<body>\n";

    ticket_file<<"\t<div id=\"main_border\">\n";
    ticket_file<<"\t\t<div id=\"titles\">\n";
    ticket_file<<"\t\t\t<div id=\"ticket_title\">\n";
    ticket_file<<"\t\t\t\tTicket\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div id=\"ticket_text\">\n";
    ticket_file<<"\t\t\t\tgenerated through\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div id=\"fbs_title\">\n";
    ticket_file<<"\t\t\t\tFlight Booking System\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div class=\"line\"></div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div class=\"airline\">\n";
    ticket_file<<"\t\t\t"<<_details._airplane.get_airline_name()<<"\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\t\t<div class=\"flight_details\">\n";
    ticket_file<<"\t\t\t<div class=\"from_to_box\">\n";
    ticket_file<<"\t\t\t\t<div class=\"from_to_text\">\n";
    ticket_file<<"\t\t\t\t\tFrom\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t\t<div class=\"city\">\n";
    ticket_file<<"\t\t\t\t\t"<<starting_airport->get_airport_city()<<" ("<<starting_airport->get_airport_code()<<")\n";
    // ticket_file<<"\t\t\t\t\tPune (PNQ)\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t\t<div class=\"airport_name\">\n";
    ticket_file<<"\t\t\t\t\t"<<starting_airport->get_airport_name()<<"\n";
    // ticket_file<<"\t\t\t\t\tPune International Airport\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t\t<div class=\"time\">\n";
    ticket_file<<"\t\t\t\t\tDeparture Time - "<<d_time[0]<<d_time[1]<<":"<<d_time[2]<<d_time[3]<<"\n";
    // ticket_file<<"\t\t\t\t\tDeparture Time - 08:00 PM\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div>\n";
    ticket_file<<"\t\t\t\t<!-- image taken from vecteezy.com -->\n";
    ticket_file<<"\t\t\t\t<img src=\"airplane_image.png\" alt=\"image\" class=\"flight_image\">\n";
    ticket_file<<"\t\t\t\t<div class=\"distance\">\n";
    ticket_file<<"\t\t\t\t\tDistance : "<<_details._airplane.get_distance()<<" KM\n";
    // ticket_file<<"\t\t\t\t\tDistance : 1245 KM\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"from_to_box from_to_box1\">\n";
    ticket_file<<"\t\t\t\t<div class=\"from_to_text\">\n";
    ticket_file<<"\t\t\t\t\tTo\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t\t<div class=\"city\">\n";
    ticket_file<<"\t\t\t\t\t"<<destination_airport->get_airport_city()<<" ("<<destination_airport->get_airport_code()<<")\n";
    // ticket_file<<"\t\t\t\t\tBhubaneswar (BBI)\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t\t<div class=\"airport_name\">\n";
    ticket_file<<"\t\t\t\t\t"<<destination_airport->get_airport_name()<<"\n";
    // ticket_file<<"\t\t\t\t\tBiju Patnaik International Airport\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t\t<div class=\"time\">\n";
    ticket_file<<"\t\t\t\t\tArrival Time - "<<a_time[0]<<a_time[1]<<":"<<a_time[2]<<a_time[3]<<"\n";
    // ticket_file<<"\t\t\t\t\tArrival Time - 10:00 PM\n";
    ticket_file<<"\t\t\t\t</div>\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div class=\"line\"></div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div id=\"passenger_title\">\n";
    ticket_file<<"\t\t\tPassenger Details\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div id=\"passenger_details\">\n";
    ticket_file<<"\t\t\t<div class=\"passenger_name passenger_details_titles\">\n";
    ticket_file<<"\t\t\t\tName\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"age passenger_details_titles\">\n";
    ticket_file<<"\t\t\t\tAge\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"gender passenger_details_titles\">\n";
    ticket_file<<"\t\t\t\tGender\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"seat passenger_details_titles\">\n";
    ticket_file<<"\t\t\t\tSeat Number\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div id=\"passenger_details\">\n";
    ticket_file<<"\t\t\t<div class=\"passenger_name\">\n";
    ticket_file<<"\t\t\t\t"<<_details.passenger_name<<"\n";
    // ticket_file<<"\t\t\t\tRAVILISETTY MAKARANDH\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"age\">\n";
    ticket_file<<"\t\t\t\t"<<_details.age<<"\n";
    // ticket_file<<"\t\t\t\t55\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"gender\">\n";
    ticket_file<<"\t\t\t\t"<<_details.gender<<"\n";
    // ticket_file<<"\t\t\t\tM\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"seat\">\n";
    ticket_file<<"\t\t\t\tG5/ECONOMY\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div class=\"line\"></div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div class=\"booking\">\n";
    ticket_file<<"\t\t\tBooking ID : 12345678910\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\t\t<div class=\"cost\">\n";
    ticket_file<<"\t\t\t<div class=\"cost_text\">\n";
    ticket_file<<"\t\t\t\tTicket Cost\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"cost_text cost_value\">\n";
    ticket_file.setf(ios::fixed,ios::floatfield);
    ticket_file<<"\t\t\t\tRs. "<<setprecision(2)<<ticket_cost<<"\n";
    // ticket_file<<"\t\t\t\t5000\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\t\t<div class=\"cost\">\n";
    ticket_file<<"\t\t\t<div class=\"cost_text\">\n";
    ticket_file<<"\t\t\t\tGST (5%)\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"cost_text cost_value\">\n";
    ticket_file<<"\t\t\t\tRs. "<<setprecision(2)<<gst<<"\n";
    // ticket_file<<"\t\t\t\t200\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\t\t<div class=\"cost leavemargin\">\n";
    ticket_file<<"\t\t\t<div class=\"cost_text\">\n";
    ticket_file<<"\t\t\t\tTotal Cost\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t\t<div class=\"cost_text cost_value\">\n";
    ticket_file<<"\t\t\t\tRs. "<<setprecision(2)<<total_cost<<"\n";
    // ticket_file<<"\t\t\t\t5200\n";
    ticket_file<<"\t\t\t</div>\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div class=\"line \"></div>\n";
    ticket_file<<"\n";
    ticket_file<<"\t\t<div id=\"thankyou\">\n";
    ticket_file<<"\t\t\tThank you for using Flight Booking System! Happy journey!\n";
    ticket_file<<"\t\t</div>\n";
    ticket_file<<"\t</div>\n";

    ticket_file<<"</body>\n";

    ticket_file<<"</html>";

    ticket_file.close();
    airplaneAnimation();
}

void bookPassengerDetails(ticket_details& _details){
    system("cls");
    int line=0;
    char yesno;

    line+=printBasicScreen(14);
    // line+=printOutputSetCursor("Passenger Details",align::center,line);
    setCursorPosition(0,line);
    line+=printLine("Passenger Details",align::center,0);
    line+=printLine();
    line+=printLine(line::dashed);
    line+=printLine();
    line+=takeInputSetCursor("Name   : ",_details.passenger_name,sizeof(_details.passenger_name),line);
    convertToUppercase(_details.passenger_name);
    line+=printOutputSetCursor("('M' for Male, 'F' for female, 'O' for other)",line);
    line+=takeInputSetCursor("Gender : ",_details.gender,sizeof(_details.gender),line);
    if(_details.gender[0]=='0') _details.gender[0]='O';
    convertToUppercase(_details.gender);
    if(_details.gender[0]!='M' && _details.gender[0]!='F') _details.gender[0]='O';
    line+=takeInputSetCursor("Age    : ",_details.age,line);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        bookPassengerDetails(_details);
        return;
    }
    else if(yesno=='q'){
        if(user_type=='G') curr_screen=screen::guest_homepage;
        else if(user_type=='U') curr_screen=screen::user_homepage;
        return;
    }
    printTicket(_details);
}

void bookTicketFlightList(ticket_details& _details){
    setCursorVisibility(false);
    system("cls");
    int line=0,o_flights_size,airplane_list_size,input_num,adjust_lines=0;
    char _route_code[2*SMALL_SIZE+1],text[MEDIUM_SIZE+1];
    char *temp_code;
    vector<airplane*> airplane_list,o_flights;
    airport* _s_airport=(code_to_airport[_details.s_airport]);
    o_flights=_s_airport->get_outgoing_flights();
    o_flights_size=o_flights.size();
    strcpy(_route_code,_details.s_airport);
    strcpy(_route_code+SMALL_SIZE,_details.d_airport);
    for(int i=0;i<o_flights_size;i++){
        temp_code=(o_flights[i])->get_route_code();
        if(strcmp(_route_code,temp_code)==0) airplane_list.push_back(o_flights[i]);
    }
    airplane_list_size=airplane_list.size();

    line+=printTopBorder();
    line+=printTitle();
    line+=printLine();
    line+=printLine();
    line+=printLine(line::dashed);
    line+=printLine();
    line+=printLine("Available Flights",align::center,0);
    line+=printLine();
    line+=printLine(line::dashed);

    for(int i=0;i<airplane_list_size;i++){
        printLine();
        sprintf(text,"%d.",i+1);
        printLine(text,colour::white);
        (airplane_list[i])->display();
        if(i!=airplane_list_size-1){
            printLine();
            printLine(line::dotted);
        }
    }
    if(airplane_list_size==0){
        line+=printLine();
        line+=printOutputSetCursor("There are no available flights.",line);
    }
    printLine();
    printLine(line::dashed);
    printLine();
    adjust_lines+=printBottomBorder();
    adjust_lines+=printLine();
    adjust_lines+=printLine();

    pair<int,int> cursor_pos;
    getCursorPosition(cursor_pos);
    cursor_pos.second-=adjust_lines;
    line=cursor_pos.second;
    setCursorPosition(cursor_pos);
    for(int i=0;i<adjust_lines-1;i++) printLine();
    printBottomBorder();
    if(curr_screen==screen::flight_availability || airplane_list_size==0){
        if(user_type=='G') curr_screen=screen::guest_homepage;
        else if(user_type=='U') curr_screen=screen::user_homepage;
        // printOutputSetCursor("Press Escape to go back",line);
        setCursorPosition(0,line);
        printLine("Press Escape to go back",colour::red);
        cout<<"\n\n";
        inputEscape();
        setCursorVisibility(true);
        return;
    }
    setCursorVisibility(true);
    line+=takeInputSetCursor("Enter Serial number of the flight : ",input_num,line);

    if(input_num<1 || input_num>airplane_list_size){
        bookTicketFlightList(_details);
        setCursorVisibility(true);
        return;
    }
    _details._airplane=(*airplane_list[input_num-1]);
    // for(int i=0;i<airplane_list_size;i++){
    //     if(i+1==input_num){
    //         _details._airplane=(*airplane_list[i]);
    //         break;
    //     }
    // }
    bookPassengerDetails(_details);
}

void bookTicketFlights(){
    system("cls");
    int line=0;
    char yesno;
    ticket_details _details;

    line+=printBasicScreen(15);
    // line+=printOutputSetCursor("Flight Details",align::center,line);
    setCursorPosition(0,line);
    line+=printLine("Flight Details",align::center,0);
    line+=printLine();
    line+=printLine(line::dashed);
    line+=printLine();

    line+=takeInputSetCursor("Starting Airport Code    : ",_details.s_airport,sizeof(_details.s_airport),line);
    convertToUppercase(_details.s_airport);
    line+=takeInputSetCursor("Destination Airport Code : ",_details.d_airport,sizeof(_details.d_airport),line);
    convertToUppercase(_details.d_airport);
    line+=printOutputSetCursor("Are the above details correct ?",line);
    line+=takeInputSetCursor("Press 'Y' for YES , 'N' for NO and 'Q' to Quit : ",yesno,line);
    if(yesno=='n'){
        bookTicketFlights();
        return;
    }
    else if(yesno=='q'){
        if(user_type=='G') curr_screen=screen::guest_homepage;
        else if(user_type=='U') curr_screen=screen::user_homepage;
        return;
    }
    bookTicketFlightList(_details);
}

void passwordInput(char *s,int size){
    fflush(stdin);
    char c='a';
    int l=0;
    // check why '\n'=10 and enter = 13 and why this problem isn't there for cin.get()
    // cout<<(int)'\n';
    while(l<size-1){
        c=getch();
        // cout<<(int)c;
        if(c==(int)13) break;
        // if(c=='\n') break;
        s[l++]=c;
        cout<<"*";
    }
    s[l]='\0';
    while(c!=(int)13) c=getch();
    cout<<"\n";
}

void loginScreen(){
    system("cls");
    int line=0;
    char _user_id[MEDIUM_SIZE+1],_password[MEDIUM_SIZE+1];

    line+=printBasicScreen(10);
    setCursorPosition(LEFT_BORDER_WIDTH+1+TEXT_PADDING,line);
    cout<<"User ID  : ";
    line++;
    customInput(_user_id,sizeof(_user_id));
    setCursorPosition(LEFT_BORDER_WIDTH+1+TEXT_PADDING,line);
    cout<<"Password : ";
    line++;
    passwordInput(_password,sizeof(_password));
    // cout<<_user_id<<" "<<_password<<"\n";
}

void controlCenter(){
    curr_screen=screen::usage_type;
    pthread_t guestScreen_thread,printUserScreen_thread,printUserTypeSelectionScreen_thread,takeInput_thread,printAdminScreen_thread,printLoggedOutScreen_thread,manageAirportsScreen_thread,manageAirlinesScreen_thread,manageAirplaneModelsScreen_thread,manageRoutesScreen_thread,manageAirplanesScreen_thread,manageImpUpdatesScreen_thread;

    while(true){
        if(curr_screen==program_exit) break;
        switch(curr_screen){
            case screen::usage_type :
                pthread_create(&printUserTypeSelectionScreen_thread,NULL,printUserTypeSelectionScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(printUserTypeSelectionScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::guest_homepage :
                pthread_create(&guestScreen_thread,NULL,guestScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(guestScreen_thread,NULL);
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
            case screen::manage_imp_updates :
                pthread_create(&manageImpUpdatesScreen_thread,NULL,manageImpUpdatesScreen,NULL);
                pthread_create(&takeInput_thread,NULL,takeInput,NULL);
                pthread_join(manageImpUpdatesScreen_thread,NULL);
                pthread_join(takeInput_thread,NULL);
                break;
            case screen::add_imp_update :
                addImpUpdateScreen();
                break;
            case screen::delete_imp_update :
                deleteImpUpdateScreen();
                break;
            case screen::imp_update_list :
                impUpdateListScreen();
                break;
            case screen::book_ticket:
                bookTicketFlights();
                break;
            case screen::flight_availability:
                bookTicketFlights();
                break;
            case screen::program_exit :
                break;
            default :
                break;
        }
    }

    system("cls");
}

// check if all input characters are trie characters are not
// subsequent attempt to check flight availability is causing some error
//      - I think the bug was due to use of (*). rather than ()-> , that to in class airport only
//      - the bug has been fixed