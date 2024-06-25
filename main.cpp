#include "backend.cpp"

int main(){
    initializeDataFromFiles();
    controlCenter();
    cout<<"\n\n\n\n\n";
    resetStyle();
    return 0;
}