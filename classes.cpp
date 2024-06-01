#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <map>
#define SMALL_SIZE 3
#define MEDIUM_SIZE 50
#define LARGE_SIZE 100
#define TRIE_CHARACTERS 63      // 52 Alphabets + 10 Digits + Whitespace
using namespace std;

class airline;
class airplane_model;
class route;
class airplane;
class airport;
template<class T>
class trie;
template<class T>
struct trieNode;

// check how to use stl containers with classes (like vector<airplane>)
// map<string,airline*> name_to_airline;
// map<string,airplane_model*> name_to_airplane_model;
// map<string,airport*> code_to_airport;
// map<string,route*> code_to_route;

template<class T>
struct trieNode{
    T* value;
    bool isEnd;
    trieNode<T>* next[TRIE_CHARACTERS];     // A to Z , a to z , , 0 to 9 , ' '

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
        else if(ch==' ') return TRIE_CHARACTERS;                // 62
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

    trieProxy operator[](const char *key){
        return trieProxy(this,key);
    }
};

trie<airline> name_to_airline;
trie<airplane_model> name_to_airplane_model;
trie<airport> code_to_airport;
trie<route> code_to_route;

class airline{
protected:
    char airline_name[MEDIUM_SIZE+1];

public:
    airline(){
        airline_name[0]='\0';
    }

    airline(char *a_name){
        strcpy(airline_name,a_name);
    }

    char* get_airline_name(){
        return airline_name;
    }

    void tempDisplay(){
        cout<<airline_name<<endl;
    }
};

class airplane_model{
protected:
    int passenger_capacity;
    char model_name[MEDIUM_SIZE+1];

public:
    airplane_model(){
        passenger_capacity=0;
        model_name[0]='\0';
    }

    airplane_model(int capacity, char *m_name){
        passenger_capacity=capacity;
        strcpy(model_name,m_name);
    }

    int get_passenger_capacity(){
        return passenger_capacity;
    }

    char* get_model_name(){
        return model_name;
    }

    void tempDisplay(){
        cout<<model_name<<" "<<passenger_capacity<<endl;
    }
};

class route{
protected:
    int route_distance;
    char starting_airport[SMALL_SIZE+1];
    char destination_airport[SMALL_SIZE+1];
    char route_code[2*SMALL_SIZE+1];

public:
    route(){
        route_distance=0;
        starting_airport[0]='\0';
        destination_airport[0]='\0';
        route_code[0]='\0';
    }

    route(int distance, char *start, char *end){
        route_distance=0;
        strcpy(starting_airport,start);
        strcpy(destination_airport,end);
        strcpy(route_code,starting_airport);
        strcpy(route_code+SMALL_SIZE,destination_airport);
    }

    char* get_route_code(){
        return route_code;
    }

    void tempDisplay(){}
};

class airplane : public airline, public airplane_model, public route{
protected:
    int airplane_cost;
    int departure_time; // departure from starting_airport
    int arrival_time;   // arrival at destination_airport

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
};

class airport{
protected:
    char airport_name[LARGE_SIZE+1];
    char airport_city[MEDIUM_SIZE+1];
    char airport_code[SMALL_SIZE+1];
    vector<airplane*> outgoing_flights;

public:
    airport(){
        airport_name[0]='\0';
        airport_city[0]='\0';
        airport_code[0]='\0';
        outgoing_flights.clear();
    }

    airport(char *a_name, char *a_city, char *a_code){
        strcpy(airport_name,a_name);
        strcpy(airport_city,a_city);
        strcpy(airport_code,a_code);
    }

    airport(char *a_name, char *a_city, char *a_code, vector<airplane*>& o_flights){
        strcpy(airport_name,a_name);
        strcpy(airport_city,a_city);
        strcpy(airport_code,a_code);
        outgoing_flights=o_flights;
    }

    ~airport(){
        outgoing_flights.clear();
    }

    char* get_airport_code(){
        return airport_code;
    }

    // void tempDisplay(){
    //     cout<<airport_name<<endl;
    //     cout<<airport_city<<endl;
    //     cout<<airport_code<<endl;
    //     cout<<endl;
    // }
    void tempDisplay(){}

};

void addAirline(){
    fflush(stdin);
    char a_name[MEDIUM_SIZE+1];
    cout<<"Airline : ";
    cin.getline(a_name,sizeof(a_name));
    airline *_airline;
    fstream airline_file;
    airline_file.open("airline_data.bin",ios::out | ios::app | ios::binary);
    _airline = new airline(a_name);
    airline_file.seekp(0,ios::beg);
    airline_file.write((char*)_airline,sizeof(airline));
    name_to_airline[a_name]=_airline;
}

void addAirplaneModel(){
    fflush(stdin);
    char a_name[MEDIUM_SIZE+1];
    int capacity;
    cout<<"Airplane Model : ";
    cin.getline(a_name,sizeof(a_name));
    fflush(stdin);
    cout<<"Passenger Capacity : ";
    cin>>capacity;
    airplane_model *_airplane_model;
    fstream airplane_model_file;
    airplane_model_file.open("airplane_model_data.bin",ios::out | ios::app | ios::binary);
    _airplane_model = new airplane_model(capacity,a_name);
    airplane_model_file.seekp(0,ios::beg);
    airplane_model_file.write((char*)_airplane_model,sizeof(airplane_model));
    name_to_airplane_model[a_name]=_airplane_model;
}

void addRoute(){
    fflush(stdin);
    char s_airport[SMALL_SIZE+1],e_airport[SMALL_SIZE+1],r_code[2*SMALL_SIZE+1];
    int r_distance;
    cout<<"Starting Airport : ";
    cin.getline(s_airport,sizeof(s_airport));
    fflush(stdin);
    cout<<"Ending Airport : ";
    cin.getline(s_airport,sizeof(s_airport));
    fflush(stdin);
    cout<<"Route distance : ";
    cin>>r_distance;
    strcpy(r_code,s_airport);
    strcpy(r_code+SMALL_SIZE,e_airport);
    route *_route;
    fstream route_file;
    route_file.open("route_data.bin",ios::out | ios::app | ios::binary);
    _route = new route(r_distance,s_airport,e_airport);
    route_file.seekp(0,ios::beg);
    route_file.write((char*)_route,sizeof(route));
    code_to_route[r_code]=_route;
}

void initializeMapsFromFiles(){
    airline _airline;
    fstream airline_file;
    airline_file.open("airline_data.bin",ios::in | ios::app | ios::binary);
    airline_file.seekg(0,ios::beg);
    while(airline_file.read((char*)&_airline,sizeof(airline))){
        name_to_airline[_airline.get_airline_name()] = new airline(_airline);
    }
    airline_file.close();

    airplane_model _airplane_model;
    fstream airplane_model_file;
    airplane_model_file.open("airplane_model_data.bin",ios::in | ios::app | ios::binary);
    airplane_model_file.seekg(0,ios::beg);
    while(airplane_model_file.read((char*)&_airplane_model,sizeof(airplane_model))){
        name_to_airplane_model[_airplane_model.get_model_name()] = new airplane_model(_airplane_model);
    }
    airplane_model_file.close();

    route _route;
    fstream route_file;
    route_file.open("route_data.bin",ios::in | ios::app | ios::binary);
    route_file.seekg(0,ios::beg);
    while(route_file.read((char*)&_route,sizeof(route))){
        code_to_route[_route.get_route_code()] = new route(_route);
    }
    route_file.close();

    airport _airport;
    fstream airport_file;
    airport_file.open("airport_data.bin",ios::in | ios::app | ios::binary);
    airport_file.seekg(0,ios::beg);
    while(airport_file.read((char*)&_airport,sizeof(airport))){
        code_to_airport[_airport.get_airport_code()] = new airport(_airport);
    }
    airport_file.close();
}

int main(){
    initializeMapsFromFiles();
    // addAirline();
    // addAirline();
    addAirplaneModel();
    // addAirplaneModel();
    airline *t= name_to_airline["Akasa"];
    if(t!=NULL) (*t).tempDisplay();
    airplane_model *s=name_to_airplane_model["fgh"];
    if(s!=NULL) (*s).tempDisplay();
    airplane_model *s1=name_to_airplane_model["adhkd"];
    if(s1!=NULL) (*s1).tempDisplay();
    airplane_model *s2=name_to_airplane_model["Boeing 747"];
    if(s2!=NULL) (*s2).tempDisplay();
    return 0;
}