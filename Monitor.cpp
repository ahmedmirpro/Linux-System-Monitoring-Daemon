#include "Monitor.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <csignal>
#include <sstream>

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

Monitor::ProcessCpuStats Monitor::ReadCpuStats(int pid) { // on definit la methode X de la class Monitor, de ProcessCpuStats de la class Monitor
    ProcessCpuStats stats = {0, 0, 0, 0};
    std::string stat_path = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream file(stat_path);
    if(!file.is_open()){
        LogMonitoring("Failed to open the file STAT\n");
        return stats;
    }
    
    std::string line;
    while(std::getline(file, line)){
        size_t open_parent = line.find('(');
        size_t close_parent = line.rfind(')');
        if(open_parent == std::string::npos || close_parent == std::string::npos){
            LogMonitoring("Invalid State format");
            return stats;
        }
        std::string pid_part = line.substr(0, open_parent);
        std::string comm_part = line.substr(open_parent + 1, close_parent - open_parent - 1);
        std::string rest_part = line.substr(close_parent + 2);
        std::stringstream ss(rest_part); // Créer un flux ss à partir de la string rest_part, pour pouvoir la lire comme si c’était un fichier ou un flux.
        std::string field;

        DebugLog("pid_part: " + pid_part);
        DebugLog("comm_part: " + comm_part);

        int index = 3;

        while(ss >> field){ // Lit mot par mot depuis ss et stocke chaque mot dans field ; lorsqu’il n’y a plus rien à lire, la boucle s’arrête.
            //Tant qu'il est possible de lire un mot depuis le flux ss, le stocker dans field et exécuter la boucle.
            if(index == 14){
                stats.utime = std::stol(field);
            }
            else if(index == 15){
                stats.stime = std::stol(field);
            }
            else if(index == 22){
                stats.starttime = std::stol(field);
            }
            
        index++;
        }
        stats.total_cpu_ticks = stats.utime + stats.stime;
    }
    stats.valid = true;
    return stats;
}

void Monitor::LogCpuStats(const ProcessCpuStats& stats){
    if(!stats.valid){
        LogMonitoring("Invalid CPU Stats!");
    }
    else {
        if(stats.utime != 0){
            LogMonitoring("- utime: " + std::to_string(stats.utime));
        }
        if(stats.stime != 0){
            LogMonitoring("- stime: " + std::to_string(stats.stime));
        }
        if(stats.starttime != 0){
            LogMonitoring("- starttime: " + std::to_string(stats.starttime));
        }
        if(stats.total_cpu_ticks){
            LogMonitoring("- Total CPU time: " +std::to_string(stats.total_cpu_ticks) + " ticks");
        }
        LogMonitoring("--------------------");
    }
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
        if(line.rfind("Name:", 0) == 0 || //commencer la rechecrche a partir de la pos 0
            line.rfind("State:", 0) == 0 ||
            line.rfind("Pid:", 0) == 0 ||
            line.rfind("PPid:", 0) == 0 ||
            line.rfind("VmRSS:", 0) == 0)
        {
            LogMonitoring("- " + line);
        }
    }
    LogMonitoring("--------------------");
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