#include "frontend.cpp"
#define TRIE_CHARACTERS 63      // 52 Alphabets + 10 Digits + Whitespace
using namespace std;

class ticket_details;
class airline;
class airplane_model;
class route;
class airplane;
class airport;
template<class T>
class trie;
template<class T>
struct trieNode;

template<class T>
struct trieNode{
    T* value;
    bool isEnd;
    trieNode<T>* next[TRIE_CHARACTERS];     // A to Z , a to z , 0 to 9 , ' '

    trieNode(){
        value=NULL;
        isEnd=false;
        for(int i=0;i<TRIE_CHARACTERS;i++) next[i]=NULL;
    }

    trieNode(T *T_object){
        value=T_object;
        isEnd=false;
        for(int i=0;i<TRIE_CHARACTERS;i++) next[i]=NULL;
    }
};

template<class T>
class trie{
private:
    trieNode<T> *root;

    int get_index(char ch){
        if('A'<=ch && ch<='Z') return (int)(ch-'A');            // 0 to 25
        else if('a'<=ch && ch<='z') return (int)(ch-'a'+26);    // 26 to 51
        else if('0'<=ch && ch<='9') return (int)(ch-'0'+52);    // 52 to 61
        else if(ch==' ') return TRIE_CHARACTERS-1;              // 62
        else return -1;
    }

    trieNode<T>* get_key_node(const char *key){
        trieNode<T> *temp=root;
        int key_size=strlen(key),index;
        for(int i=0;i<key_size;i++){
            index=get_index(key[i]);
            if(temp->next[index]==NULL) return NULL;
            temp=temp->next[index];
        }
        if(temp->isEnd) return temp;
        else return NULL;
    }

    class trieProxy{
    private:
        trie<T> *tr;
        const char *key;

    public:
        trieProxy(){
            tr=NULL;
            key=NULL;
        }

        trieProxy(trie<T> *_tr, const char *_key){
            tr=_tr;
            key=_key;
        }

        void operator=(T* _value){
            trieNode<T> *node=tr->get_key_node(key);
            if(node==NULL){
                tr->insert(key,_value);
            }
            else{
                node->value=_value;
            }
        }

        // this is type caster
        operator T*() const{
            return tr->get_value(key);
        }
    };

public:
    trie(){
        root = new trieNode<T>;
    }

    void insert(const char *key, T *T_value){
        trieNode<T> *temp=root;
        int key_size=strlen(key),index;
        for(int i=0;i<key_size;i++){
            index=get_index(key[i]);
            if(temp->next[index]==NULL) temp->next[index] = new trieNode<T>;
            temp=temp->next[index];
        }
        temp->isEnd=true;
        temp->value=T_value;
    }

    bool find(const char *key){
        trieNode<T> *temp=root;
        int key_size=strlen(key),index;
        for(int i=0;i<key_size;i++){
            index=get_index(key[i]);
            if(temp->next[index]==NULL) return false;
            temp=temp->next[index];
        }
        return temp->isEnd;
    }

    T* get_value(const char *key){
        trieNode<T> *temp=root;
        int key_size=strlen(key),index;
        for(int i=0;i<key_size;i++){
            index=get_index(key[i]);
            if(temp->next[index]==NULL) return NULL;
            temp=temp->next[index];
        }
        if(temp->isEnd) return temp->value;
        else return NULL;
    }

    void erase(const char *key){
        if(!find(key)) return;
        trieNode<T> *temp=root,*temp2;
        int key_size=strlen(key),index,last_useful_node=(-1),children=0;
        if(key_size==0){
            temp->isEnd=false;
            return;
        }
        for(int i=0;i<key_size;i++){
            index=get_index(key[i]);
            temp=temp->next[index];
            children=0;
            for(int i=0;i<TRIE_CHARACTERS;i++){
                if(temp->next[i]!=NULL) children++;
            }
            if((children>=2) || ((temp->isEnd) && (i!=key_size-1))) last_useful_node=i;
            if((children>=1) && (i==key_size)) last_useful_node=key_size-1;
        }
        temp->isEnd=false;
        delete temp->value;
        temp->value=NULL;
        if(last_useful_node==key_size-1) return;
        temp=root;
        for(int i=0;i<=last_useful_node;i++){
            index=get_index(key[i]);
            temp=temp->next[index];
        }
        temp2=temp;
        index=get_index(key[last_useful_node+1]);
        temp=temp->next[index];
        temp2->next[index]=NULL;
        for(int i=last_useful_node+2;i<key_size;i++){
            index=get_index(key[i]);
            temp2=temp;
            temp=temp->next[index];
            delete temp2;
        }
        delete temp;
    }

    void traverse(vector<T*>& v){
        stack<trieNode<T>*> stk;
        stk.push(root);
        while(!stk.empty()){
            trieNode<T> *temp_node=stk.top();
            stk.pop();
            if(temp_node->isEnd) v.push_back(temp_node->value);
            for(int i=TRIE_CHARACTERS-1;i>=0;i--){
                if(temp_node->next[i]!=NULL) stk.push(temp_node->next[i]);
            }
        }
    }

    void copy_to_file(const char *file_name){
        vector<T*> v;
        traverse(v);
        int size=v.size();
        fstream output_file;
        output_file.open(file_name,ios::out | ios::binary);
        output_file.seekp(0,ios::beg);
        for(int i=0;i<size;i++){
            output_file.write((char*)v[i],sizeof(T));
        }
        output_file.close();
    }

    // this function is only used for T = airport
    void copy_to_file_v2(const char *file_name){
        vector<T*> v;
        traverse(v);
        int size=v.size();
        fstream output_file;
        output_file.open(file_name,ios::out | ios::binary);
        output_file.seekp(0,ios::beg);
        for(int i=0;i<size;i++){
            v[i]->write_to_file(output_file);
        }
        output_file.close();
    }

    trieProxy operator[](const char *key){
        return trieProxy(this,key);
    }
};

trie<airline> name_to_airline;
trie<airplane_model> name_to_airplane_model;
trie<airport> code_to_airport;
trie<route> code_to_route;

#pragma pack(push, 1)
class airline{
protected:
    char airline_name[MEDIUM_SIZE+1];

public:
    airline(){
        memset((void*)airline_name, 0, sizeof(airline_name));
    }

    airline(char *a_name){
        memset((void*)airline_name, 0, sizeof(airline_name));
        strcpy(airline_name,a_name);
    }

    char* get_airline_name(){
        return airline_name;
    }

    void write_to_file(ofstream& _file){
        _file.write((char*)this, sizeof(airline));
    }

    void write_to_file(fstream& _file){
        _file.write((char*)this, sizeof(airline));
    }

    void display(){
        printLine("Airline Name             : ",airline_name,colour::green,true);
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
class airplane_model{
protected:
    int passenger_capacity;
    char model_name[MEDIUM_SIZE+1];

public:
    airplane_model(){
        passenger_capacity=0;
        memset((void*)model_name, 0, sizeof(model_name));
    }

    airplane_model(int capacity, char *m_name){
        passenger_capacity=capacity;
        memset((void*)model_name, 0, sizeof(model_name));
        strcpy(model_name,m_name);
    }

    int get_passenger_capacity(){
        return passenger_capacity;
    }

    char* get_model_name(){
        return model_name;
    }

    void write_to_file(ofstream& _file){
        _file.write((char*)this, sizeof(airplane_model));
    }

    void write_to_file(fstream& _file){
        _file.write((char*)this, sizeof(airplane_model));
    }

    void display(){
        char text[2*LARGE_SIZE+1];
        printLine("Airplane Model Name      : ",model_name,colour::green,true);
        sprintf(text,"%d",passenger_capacity);
        printLine("Passenger Capacity       : ",text,colour::green,true);
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
class route{
protected:
    int route_distance;
    char starting_airport[SMALL_SIZE+1];
    char destination_airport[SMALL_SIZE+1];
    char route_code[2*SMALL_SIZE+1];

public:
    route(){
        route_distance=0;
        memset((void*)starting_airport, 0, sizeof(starting_airport));
        memset((void*)destination_airport, 0, sizeof(destination_airport));
        memset((void*)route_code, 0, sizeof(route_code));
    }

    route(int distance, char *start, char *end){
        route_distance=distance;
        memset((void*)starting_airport, 0, sizeof(starting_airport));
        memset((void*)destination_airport, 0, sizeof(destination_airport));
        memset((void*)route_code, 0, sizeof(route_code));
        strcpy(starting_airport,start);
        strcpy(destination_airport,end);
        strcpy(route_code,starting_airport);
        strcpy(route_code+SMALL_SIZE,destination_airport);
    }

    char* get_starting_airport(){
        return starting_airport;
    }

    char* get_destination_airport(){
        return destination_airport;
    }

    char* get_route_code(){
        return route_code;
    }

    int get_distance(){
        return route_distance;
    }

    void write_to_file(ofstream& _file){
        _file.write((char*)this, sizeof(route));
    }

    void write_to_file(fstream& _file){
        _file.write((char*)this, sizeof(route));
    }

    void display(){
        char text[2*LARGE_SIZE+1];
        printLine("Starting Airport Code    : ",starting_airport,colour::green,true);
        printLine("Destination Airport Code : ",destination_airport,colour::green,true);
        printLine("Route Code               : ",route_code,colour::green,true);
        sprintf(text,"%d",route_distance);
        printLine("Route Distance (in KM)   : ",text,colour::green,true);
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
class airplane : public airline, public airplane_model, public route{
protected:
    int airplane_cost;
    int departure_time;     // departure from starting_airport
    int arrival_time;       // arrival at destination_airport

public:
    airplane() :
    airline(), airplane_model(), route(){
        airplane_cost=0;
        departure_time=0000;
        arrival_time=0000;
    }

    airplane(char *a_name, int capacity, char *model_name, int distance, char *start, char *end, int a_cost, int d_time, int a_time) :
    airline(a_name), airplane_model(capacity,model_name), route(distance,start,end){
        airplane_cost=a_cost;
        departure_time=d_time;
        arrival_time=a_time;
    }

    airplane(airline& a_line, airplane_model& a_model, route& r, int a_cost, int d_time, int a_time) :
    airline(a_line), airplane_model(a_model), route(r){
        airplane_cost=a_cost;
        departure_time=d_time;
        arrival_time=a_time;
    }

    int get_departure_time(){
        return departure_time;
    }

    int get_arrival_time(){
        return arrival_time;
    }

    int get_cost(){
        return airplane_cost;
    }

    void write_to_file(ofstream& _file){
        _file.write((char*)this, sizeof(airplane));
    }

    void write_to_file(fstream& _file){
        _file.write((char*)this, sizeof(airplane));
    }

    void display(){
        airline::display();
        airplane_model::display();
        route::display();
        char text[2*LARGE_SIZE+1];
        sprintf(text,"%02d:%d%d",departure_time/100,(departure_time%100)/10,departure_time%10);
        printLine("Departure Time           : ",text,colour::green,true);
        sprintf(text,"%02d:%d%d",arrival_time/100,(arrival_time%100)/10,arrival_time%10);
        printLine("Arrival Time             : ",text,colour::green,true);
        sprintf(text,"%d",airplane_cost);
        printLine("Ticket Cost (in Rs.)     : ",text,colour::green,true);
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
class airport{
private:
    char airport_name[LARGE_SIZE+1];
    char airport_city[MEDIUM_SIZE+1];
    char airport_code[SMALL_SIZE+1];
    vector<airplane*> outgoing_flights;

public:
    airport(){
        memset((void*)airport_name, 0, sizeof(airport_name));
        memset((void*)airport_city, 0, sizeof(airport_city));
        memset((void*)airport_code, 0, sizeof(airport_code));
        outgoing_flights.clear();
    }

    airport(char *a_name, char *a_city, char *a_code){
        memset((void*)airport_name, 0, sizeof(airport_name));
        memset((void*)airport_city, 0, sizeof(airport_city));
        memset((void*)airport_code, 0, sizeof(airport_code));
        strcpy(airport_name,a_name);
        strcpy(airport_city,a_city);
        strcpy(airport_code,a_code);
        outgoing_flights.clear();
    }

    airport(char *a_name, char *a_city, char *a_code, vector<airplane*>& o_flights){
        memset((void*)airport_name, 0, sizeof(airport_name));
        memset((void*)airport_city, 0, sizeof(airport_city));
        memset((void*)airport_code, 0, sizeof(airport_code));
        strcpy(airport_name,a_name);
        strcpy(airport_city,a_city);
        strcpy(airport_code,a_code);
        outgoing_flights=o_flights;
    }

    void operator=(airport& another){
        strcpy(this->airport_name,another.get_airport_name());
        strcpy(this->airport_city,another.get_airport_city());
        strcpy(this->airport_code,another.get_airport_code());
        (this->outgoing_flights)=another.get_outgoing_flights();
    }

    char* get_airport_city(){
        return airport_city;
    }

    char* get_airport_code(){
        return airport_code;
    }

    char* get_airport_name(){
        return airport_name;
    }

    vector<airplane*>& get_outgoing_flights(){
        return outgoing_flights;
    }

    void clear_flights(){
        outgoing_flights.clear();
    }

    void add_airplane(airplane *_airplane){
        outgoing_flights.push_back(_airplane);
    }

    void delete_airplane(char *_route_code, int _departure_time){
        vector<airplane*> new_outgoing_flights;
        int size=outgoing_flights.size();
        airplane *_airplane;
        for(int i=0;i<size;i++){
            _airplane=outgoing_flights[i];
            if((strcmp(_route_code,(_airplane)->get_route_code())==0) && (_departure_time==(_airplane)->get_departure_time())){
                delete _airplane;
            }
            else{
                new_outgoing_flights.push_back(_airplane);
            }
        }
        outgoing_flights=new_outgoing_flights;
    }

    bool find_airplane(char *_route_code, int _departure_time){
        int size=outgoing_flights.size();
        airplane *_airplane;
        for(int i=0;i<size;i++){
            _airplane=outgoing_flights[i];
            if((strcmp(_route_code,(_airplane)->get_route_code())==0) && (_departure_time==(_airplane)->get_departure_time())){
                return true;
            }
        }
        return false;
    }

    void copy_flights_to_file(const char *file_name){
        fstream output_file;
        output_file.open(file_name,ios::out | ios::app | ios::binary);
        output_file.seekp(0,ios::end);
        int size=outgoing_flights.size();
        for(int i=0;i<size;i++){
            output_file.write((char*)outgoing_flights[i],sizeof(airplane));
        }
        output_file.close();
    }

    void read_from_file(ifstream& _file){
        _file.read(airport_name, sizeof(airport_name));
        _file.read(airport_city, sizeof(airport_city));
        _file.read(airport_code, sizeof(airport_code));
    }

    void read_from_file(fstream& _file){
        _file.read(airport_name, sizeof(airport_name));
        _file.read(airport_city, sizeof(airport_city));
        _file.read(airport_code, sizeof(airport_code));
    }

    void write_to_file(ofstream& _file){
        _file.write(airport_name, sizeof(airport_name));
        _file.write(airport_city, sizeof(airport_city));
        _file.write(airport_code, sizeof(airport_code));
    }

    void write_to_file(fstream& _file){
        _file.write(airport_name, sizeof(airport_name));
        _file.write(airport_city, sizeof(airport_city));
        _file.write(airport_code, sizeof(airport_code));
    }

    void display(){
        printLine("Name : ",airport_name,colour::green,true);
        printLine("City : ",airport_city,colour::green,true);
        printLine("Code : ",airport_code,colour::green,true);
    }
};
#pragma pack(pop)

class ticket_details{
public:
    int age;
    char passenger_name[MEDIUM_SIZE+1];
    char gender[2];
    char s_airport[SMALL_SIZE+1];
    char d_airport[SMALL_SIZE+1];
    airplane _airplane;

    ticket_details(){
        age=0;
        passenger_name[0]='\0';
        gender[0]='\0';
        s_airport[0]='\0';
        d_airport[0]='\0';
        _airplane=airplane();
    }
};

void initializeDataFromFiles(){
    airline _airline;
    fstream airline_file;
    airline_file.open("Data/airline_data.bin",ios::in | ios::app | ios::binary);
    airline_file.seekg(0,ios::beg);
    while(airline_file.read((char*)&_airline,sizeof(airline))){
        name_to_airline[_airline.get_airline_name()] = new airline(_airline);
        // (*name_to_airline[_airline.get_airline_name()]).display();
    }
    airline_file.close();

    airplane_model _airplane_model;
    fstream airplane_model_file;
    airplane_model_file.open("Data/airplane_model_data.bin",ios::in | ios::app | ios::binary);
    airplane_model_file.seekg(0,ios::beg);
    while(airplane_model_file.read((char*)&_airplane_model,sizeof(airplane_model))){
        name_to_airplane_model[_airplane_model.get_model_name()] = new airplane_model(_airplane_model);
        // (*name_to_airplane_model[_airplane_model.get_model_name()]).display();
    }
    airplane_model_file.close();

    route _route;
    fstream route_file;
    route_file.open("Data/route_data.bin",ios::in | ios::app | ios::binary);
    route_file.seekg(0,ios::beg);
    while(route_file.read((char*)&_route,sizeof(route))){
        code_to_route[_route.get_route_code()] = new route(_route);
        // (*code_to_route[_route.get_route_code()]).display();
    }
    route_file.close();

    fstream airport_file;
    airport_file.open("Data/airport_data.bin",ios::in | ios::app | ios::binary);
    airport_file.seekg(0,ios::end);
    int total_bytes=airport_file.tellg();
    airport_file.seekg(0,ios::beg);
    while(airport_file.tellg() != total_bytes){
        airport _airport;
        _airport.read_from_file(airport_file);
        _airport.clear_flights();
        code_to_airport[_airport.get_airport_code()] = new airport(_airport);
        // (*code_to_airport[_airport.get_airport_code()]).display();
    }
    airport_file.close();

    airplane _airplane;
    airport* __airport;
    fstream airplane_file;
    airplane_file.open("Data/airplane_data.bin",ios::in | ios::app | ios::binary);
    airplane_file.seekg(0,ios::beg);
    while(airplane_file.read((char*)&_airplane,sizeof(airplane))){
        __airport=code_to_airport[_airplane.get_starting_airport()];
        __airport->add_airplane(new airplane(_airplane));
        // _airplane.display();
    }
    airplane_file.close();
}

void customInput(char *s,int size){
    fflush(stdin);
    setBackgroundColour();
    char c='a';
    int l=0;
    while(l<size-1){
        c=cin.get();
        if(c=='\n') break;
        s[l++]=c;
    }
    s[l]='\0';
    if(c!='\n'){
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    resetBackgorund();
}