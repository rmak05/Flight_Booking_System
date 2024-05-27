#include <iostream>
#include <vector>
#include <cstring>
#define SMALL_SIZE 3
#define MEDIUM_SIZE 50
#define LARGE_SIZE 100
using namespace std;

class airline;
class airplane_model;
class route;
class airplane;
class airport;

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

    airline(airline& another_airline){
        strcpy(this->airline_name,another_airline.airline_name);
    }

    char* get_airline_name(){
        return airline_name;
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

    airplane_model(int capacity, char *model_name){
        passenger_capacity=capacity;
        strcpy(model_name,model_name);
    }

    airplane_model(airplane_model& another_model){
        this->passenger_capacity=another_model.passenger_capacity;
        strcpy(this->model_name,another_model.model_name);
    }

    int get_passenger_capacity(){
        return passenger_capacity;
    }

    char* get_model_name(){
        return model_name;
    }
};

class route{
protected:
    int route_distance;
    char starting_airport[SMALL_SIZE+1];
    char destination_airport[SMALL_SIZE+1];

public:
    route(){
        route_distance=0;
        starting_airport[0]='\0';
        destination_airport[0]='\0';
    }

    route(int distance, char *start, char *end){
        route_distance=0;
        strcpy(starting_airport,start);
        strcpy(destination_airport,end);
    }

    route(route& another_route){
        this->route_distance=another_route.route_distance;
        strcpy(this->starting_airport,another_route.starting_airport);
        strcpy(this->destination_airport,another_route.destination_airport);
    }
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
    
    // void operator =(airplane another_airplane){
    //     this->passenger_capacity=another_airplane.passenger_capacity;
    //     this->route_distance=another_airplane.route_distance;
    //     this->airplane_cost=another_airplane.airplane_cost;
    //     this->departure_time=another_airplane.departure_time;
    //     this->arrival_time=another_airplane.arrival_time;
    //     strcpy(this->airline_name,another_airplane.airline_name);
    //     strcpy(this->model_name,another_airplane.model_name);
    //     strcpy(this->starting_airport,another_airplane.starting_airport);
    //     strcpy(this->destination_airport,another_airplane.destination_airport);
    // }
};

class airport{
protected:
    char airport_name[LARGE_SIZE+1];
    char airport_city[MEDIUM_SIZE+1];
    char airport_code[SMALL_SIZE+1];
    vector<airplane> outgoing_flights;

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

    airport(char *a_name, char *a_city, char *a_code, vector<airplane>& o_flights){
        strcpy(airport_name,a_name);
        strcpy(airport_city,a_city);
        strcpy(airport_code,a_code);
        int o_flights_size=o_flights.size();
        outgoing_flights=vector<airplane>(o_flights_size);
        for(int i=0;i<o_flights_size;i++){
            outgoing_flights[i]=o_flights[i];
        }
    }

    ~airport(){
        outgoing_flights.clear();
    }
};

int main(){
    return 0;
}