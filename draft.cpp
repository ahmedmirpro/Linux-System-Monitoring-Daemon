#include <iostream>
#include <ctime>
#include <csignal>
#include <fstream>

using namespace std;

void Logging(){
    ofstream logger;
    logger.open("Monitor.log", std::ios_base::app);
    logger << "Updating Monitor.log\n";
    logger.close();
}

int main(){
    for (int i = 0 ; i < 5 ; i++){
        Logging();
    }
    return 0;
}