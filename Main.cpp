#include "Monitor.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <csignal>

using namespace std;

volatile sig_atomic_t keep_running = 1;
void sigint_handler(int sig);

void sigint_handler(int sig){
    (void) sig;
    keep_running = 0;
}

int main(int argc, char *argv[]){

    Monitor mnt;
    int pid = 0;
    int interval = 0;
    #ifdef DEBUG_MODE
        mnt.EnableDebug(true);
    #else
        mnt.EnableDebug(false);
    #endif

    signal(SIGINT, sigint_handler);

    if(argc !=3){ // make sure the user did not type more than 2 argument
        mnt.LogMonitoring("Usage: ./monitor <pid> <interval_seconds>\n");
        return 1;
    }
    try{
        pid = std::stoi(argv[1]); // convert the second arg from char to int (to be easily manipulated)
        interval = std::stoi(argv[2]); // convert the 3rd arg from string to int (intervall in seconds in this case)
    }
    catch(const std::exception& e){
        mnt.LogMonitoring("Invalid Arguments: PID and INTERVAL must be integers!");
        return 1;
    }

    if(pid <=0){
        mnt.LogMonitoring("Invalid PID");
        return 1;
    }
    if(interval <= 0){
        mnt.LogMonitoring("Invalid interval");
        return 1;
    }

    // if(std::filesystem::exists(proc_path)){ // check if the path built exists in the filesystem
    while(keep_running && mnt.ProcessExists(pid)){ // access(proc_path.c_str(), F_OK) checks whether the path stored in the std::string proc_path exists in the filesystem by converting it to a const char*; it returns 0 if the file/directory exists and -1 otherwise.
        mnt.LogMonitoring("[PID exists]\n");
        mnt.DisplayProcess(mnt.ReturnDatetime());
        sleep(interval);
    }

    if(!keep_running){
        mnt.LogMonitoring("\nStopping monitor gracefully...");
    }
    else {
        mnt.LogMonitoring("Process no longer exists!\n");
    }
    return 0;
}