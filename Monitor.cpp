#include "Monitor.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <csignal>

using namespace std;

std::string Monitor::ReturnDatetime(){
    time_t rawtime; // cumul dial timestamp since 1970
    char formatted_time[50]; // string dial lwaqt w date li ghadi yt2afficha
    struct tm* datetime; // structure dial conversion mn timestamp l datetime // to explain why * after tm

    time(&rawtime); // to explain why &

    datetime = localtime(&rawtime);

    strftime(formatted_time, 50, "%F %T", datetime);
    return std::string(formatted_time);
}

bool Monitor::ProcessExists(int pid){
    proc_path = "/proc/" + std::to_string(pid); // build the process path
    // std::cout << "Checking path: " << proc_path << std::endl; // debug log to make sure the pid exists and print it in the terminal
    DebugLog("Checking path: " + proc_path);
    return(access(proc_path.c_str(), F_OK)==0);
}

int Monitor::DisplayProcess(const std::string& formatted_time){
    status_path = proc_path + "/status";
    std::ifstream file(status_path);
    if(!file.is_open()){
        LogMonitoring("Failed to open the file Status\n");
        return 1;
    }
    std::string line; //pid_name
    LogMonitoring("-----PID Info-----\n");
    LogMonitoring("["+ formatted_time +"]");
    while(std::getline(file, line)){ //pid_name // std::getline(file, line) كتقرا سطر واحد من الملف وتحطو فـ line، وifstream كيبقى حافظ داخلياً على position ديال القراءة (cursor)، لذلك كل مرة كتعاود تتنادى داخل while كتقرا automatiquement السطر اللي من بعد حتى توصل لنهاية الملف، وفي هاد اللحظة getline() كترجع false وكتوقف loop.
        if(line.rfind("Name:", 0) == 0 ||
            line.rfind("State:", 0) == 0 ||
            line.rfind("Pid:", 0) == 0 ||
            line.rfind("PPid:", 0) == 0 ||
            line.rfind("VmRSS:", 0) == 0)
        {
            LogMonitoring(line);
        }
    }
    LogMonitoring("--------------------\n");
    return 0;
}

void Monitor::EnableDebug(bool enabled){
    debug_enabled = enabled;
}

void Monitor::DebugLog(const std::string& message){
    if(debug_enabled){
        LogMonitoring("[DEBUG] - " + message);
    }
}

void Monitor::LogMonitoring(const std::string& ss){
    std::cerr << ss << std::endl;
    ofstream logger;
    logger.open("Monitor.log", std::ios_base::app);
    if(!logger.is_open()){
        std::cerr << "Cannot open Monitor.log file\n";
        return;
    }
    logger << ss << std::endl;
    logger.close();
}