#include "Monitor.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <csignal>

using namespace std;

volatile sig_atomic_t keep_running = 1; // volatile means this signal might be updated by a 3rd part outside the code (system)

void sigint_handler(int sig){
    (void) sig;
    keep_running = 0;
}

int main(int argc, char *argv[]){

    Monitor mnt;
    int pid = 0;
    int interval = 0;
    #ifdef DEBUG_MODE
        int stat_index = 0;
        mnt.EnableDebug(true);
    #else
        mnt.EnableDebug(false);
    #endif

    signal(SIGINT, sigint_handler); // equivalent to sigint_handler(SIGINT) = to be checked

    #ifndef DEBUG_MODE
    if(argc !=3){ // make sure the user did not type more than 2 argument
        mnt.LogMonitoring("Usage: ./monitor <pid> <interval_seconds>\n");
        return 1;
    }
    #endif

    #ifdef DEBUG_MODE
        if(argc !=4){ // make sure the user did not type more than 2 argument
        mnt.LogMonitoring("Usage: ./monitor <pid> <interval_seconds> <STAT_index>\n");
        return 1;
    }
    #endif

    try{
        pid = std::stoi(argv[1]); // convert the second arg from char to int (to be easily manipulated)
        interval = std::stoi(argv[2]); // convert the 3rd arg from string to int (intervall in seconds in this case)
        #ifdef DEBUG_MODE
        stat_index = std::stoi(argv[3]);
        #endif
    }
    catch(const std::exception& e){
        mnt.LogMonitoring("Invalid Arguments: PID, INTERVAL and STAT_IDX must be integers!");
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

    #ifdef DEBUG_MODE
    if(stat_index <=0){
        mnt.LogMonitoring("Invalid STAT index!");
        return 1;
    }
    #endif

    // if(std::filesystem::exists(proc_path)){ // check if the path built exists in the filesystem
    while(keep_running && mnt.ProcessExists(pid)){ // access(proc_path.c_str(), F_OK) checks whether the path stored in the std::string proc_path exists in the filesystem by converting it to a const char*; it returns 0 if the file/directory exists and -1 otherwise.
        mnt.LogMonitoring("[PID exists]\n");
        mnt.DisplayProcess(mnt.ReturnDatetime());
        auto stats = mnt.ReadCpuStats(pid); //mieux vaut lire directement la structure revoyée et la passer en param en LogCpuStats, dans ce cas pas besoin d'init dans HPP de l'objet de la structure PCS
        mnt.LogCpuStats(stats);
        //mnt.DisplayCpuRawStats(pid, stat_index);
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