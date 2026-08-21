#include "Monitor.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <csignal>
#include <sstream>

using namespace std; // no real need since we need to type it each time for professional reasons

std::string Monitor::ReturnDatetime(){
    time_t rawtime; // rawtime variable qui stocke un temps/date sous une forme numérique
    char formatted_time[50]; // string dial lwaqt w date li ghadi yt2afficha
    struct tm* datetime; // structure dial conversion mn timestamp l datetime // tm* est un type structure qui renvoie un pointeur de type structure

    time(&rawtime); // rawtime apres update ici contient le temps courant représenté comme un nombre de secondes par rapport à une origine temporelle — sur les systèmes Unix/Linux, généralement l'Unix Epoch : 1er janvier 1970 à 00:00:00 UTC.

    datetime = localtime(&rawtime); // katconverti w t3emmer les elements de la structure datetime (AA::MM:JJ:hh:mm:ss) a partir men cumul de seconds li stocké f rawtime
        // localtime renvoie un pointeur vers une structure, donc datetime doit etre une strucutre du meme type pour pouvoir manipuler les elements correctement
    strftime(formatted_time, 50, "%F %T", datetime); // had method katekteb date w time b format readable, katsenna menek taatiha buff d'affichage, buff size, params dial date %F w time %T, w pointeur dial structure li fiha data dial date w time
    return std::string(formatted_time);
}

Monitor::ProcessCpuStats Monitor::ReadCpuStats(int pid) { // ici on definit une method qui renvoie une structure de type ProcessCpuStats
    ProcessCpuStats stats = {0, 0, 0, 0, 0};
    long ticks_per_seconds = sysconf(_SC_CLK_TCK);
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
        std::stringstream ss(rest_part); // Lit mot par mot depuis ss et stocke chaque mot dans field ; lorsqu’il n’y a plus rien à lire, la boucle s’arrête.
        std::string field;

        DebugLog("pid_part: " + pid_part);
        DebugLog("comm_part: " + comm_part);

        int index = 3;

        while(ss >> field){ // Lit mot par mot depuis ss et stocke chaque mot dans field ; lorsqu’il n’y a plus rien à lire, la boucle s’arrête.
            // if(index == 14 || index == 15 || index == 22 || index == idx){
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
        stats.total_cpu_seconds = static_cast<double>(stats.total_cpu_ticks) / ticks_per_seconds; // static_cast<double = (double), 1st one is C++ style and 2nd one is C-Style | (double) is a generic casting that can do a lot of unknown conversions which is not safe, unlike static_cast<double> that tell the compiler: I want a clear and simple conversion during the compilation time from long to double
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
        if(stats.total_cpu_seconds){
            LogMonitoring("- Total CPU time (seconds): " + std::to_string(stats.total_cpu_seconds));
        }
        LogMonitoring("----------------------------------------");
    }
}

bool Monitor::ProcessExists(int pid){
    proc_path = "/proc/" + std::to_string(pid); // build the process path
    // std::cout << "Checking path: " << proc_path << std::endl; // debug log to make sure the pid exists and print it in the terminal
    DebugLog("Checking path: " + proc_path);
    return(access(proc_path.c_str(), F_OK)==0); // /* F_OK = Check for file existence, file path name here the return of c_str(), c_str() converts std::string to C-lang const char* type */
}

int Monitor::DisplayProcess(const std::string& formatted_time){
    status_path = proc_path + "/status";
    std::ifstream file(status_path); // file here is an object linked to the content of "status_path"
    if(!file.is_open()){
        LogMonitoring("Failed to open the file Status\n");
        return 1;
    }
    std::string line; //pid_name
    LogMonitoring("---------------[PID Info]---------------\n");
    LogMonitoring("["+ formatted_time +"]");
    while(std::getline(file, line)){ //قرا سطر واحد من file وخزّنو فـ line.
        if(line.rfind("Name:", 0) == 0 || // cherche "Name:" à partir de la position 0 et renvoie sa position ; == 0 vérifie donc que la ligne commence bien par "Name:".
            line.rfind("State:", 0) == 0 || // bda tqallab ala State mn position 0 w ila kanet f pos zero, return true
            line.rfind("Pid:", 0) == 0 ||
            line.rfind("PPid:", 0) == 0 ||
            line.rfind("VmRSS:", 0) == 0)
        {
            LogMonitoring("- " + line);
        }
    }
    LogMonitoring("---------------[PID Stats]---------------");
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
    std::ofstream logger; // ofstream is a C++ class specific to write in files, logger is the object
    logger.open("Monitor.log", std::ios_base::app); 
    if(!logger.is_open()){
        std::cerr << "Cannot open Monitor.log file\n";
        return;
    }
    logger << ss << std::endl;
    logger.close();
}




// Quick Notes:
// std::ifstream  → قراءة من file
// std::ofstream  → كتابة في file
// std::cout      → كتابة في terminal