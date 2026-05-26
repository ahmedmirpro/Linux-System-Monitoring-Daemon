#include "Monitor.hpp"
#include <iostream>
#include <ctime>
#include <csignal>
#include <fstream>
#include <sstream>


Monitor::ProcessCpuStats Monitor::ReadCpuStats(int pid) { // on definit la methode X de la class Monitor, de ProcessCpuStats de la class Monitor
    Monitor::ProcessCpuStats pcs = {0};
    std::string stat_path = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream file(stat_path);
    if(!file.is_open()){
        LogMonitoring("Failed to open the file STAT\n");
        return {1};
    }
    
    std::string line;
    while(std::getline(file, line)){
        size_t open_parent = line.find('(');
        size_t close_parent = line.rfind(')');
        if(open_parent == std::string::npos || close_parent == std::string::npos){
            LogMonitoring("Invalid State format");
            return;
        }
        std::string pid_part = line.substr(0, open_parent);
        std::string comm_part = line.substr(open_parent + 1, close_parent - open_parent - 1);
        std::string rest_part = line.substr(close_parent + 2);
        std::stringstream ss(rest_part); // Create a stringstream object initialized with 'line'
        std::string field;

        DebugLog("pid_part: " + pid_part);
        DebugLog("comm_part: " + comm_part);

        // pcs.utime = 0;
        // pcs.stime = 0;
        // pcs.starttime = 0;
        // pcs.total_cpu_ticks = 0;
        pcs.utime;
        pcs.stime;
        pcs.starttime;
        pcs.total_cpu_ticks;

        int index = 3;

        while(ss >> field){ //قرا كلمة بكلمة من ss وحطّ كل كلمة فـ field، وملي ما يبقاش ما يتقرا، وقف loop.
            // if(index == 14 || index == 15 || index == 22 || index == idx){
            if(index == 14){
                pcs.utime = std::stol(field);
                // std::cout << "utime - " << utime;
            }
            else if(index == 15){
                pcs.stime = std::stol(field);
                // std::cout << "stime - " << stime;
            }
            else if(index == 22){
                pcs.starttime = std::stol(field);
                // std::cout << "starttime - " << starttime;
            }
        index++;
        }
        pcs.total_cpu_ticks = pcs.utime + pcs.stime;
    }
    return pcs;
}

void Monitor::LogCpuStats(const ProcessCpuStats& stats){
    if(stats.utime != 0){
        LogMonitoring("utime: " + std::to_string(stats.utime));
    }
    else if(stats.stime != 0){
        LogMonitoring("stime: " + std::to_string(stats.stime));
    }
    else if(stats.starttime != 0){
        LogMonitoring("starttime: " + std::to_string(stats.starttime));
    }
    else if(stats.total_cpu_ticks){
        LogMonitoring("Total CPU time: " +std::to_string(stats.total_cpu_ticks) + " ticks");
    }
}

int Monitor::DisplayCpuRawStats(int pid, int idx){
    std::string stat_path = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream file(stat_path);
    if(!file.is_open()){
        LogMonitoring("Failed to open the file STAT\n");
        return 1;
    }
    
    std::string line;
    while(std::getline(file, line)){
        size_t open_parent = line.find('(');
        size_t close_parent = line.rfind(')');
        if(open_parent == std::string::npos || close_parent == std::string::npos){
            LogMonitoring("Invalid State format");
            return 1;
        }
        std::string pid_part = line.substr(0, open_parent);
        std::string comm_part = line.substr(open_parent + 1, close_parent - open_parent - 1);
        std::string rest_part = line.substr(close_parent + 2);
        std::stringstream ss(rest_part); // Create a stringstream object initialized with 'line'
        std::string field;

        DebugLog("pid_part: " + pid_part);
        DebugLog("comm_part: " + comm_part);

        long utime = 0;
        long stime = 0;
        long starttime = 0;

        int index = 3;

        while(ss >> field){ //قرا كلمة بكلمة من ss وحطّ كل كلمة فـ field، وملي ما يبقاش ما يتقرا، وقف loop.
            // if(index == 14 || index == 15 || index == 22 || index == idx){
            if(index == 14){
                utime = std::stol(field);
                // std::cout << "utime - " << utime;
                LogMonitoring("utime: " + std::to_string(utime));
            }
            else if(index == 15){
                stime = std::stol(field);
                // std::cout << "stime - " << stime;
                LogMonitoring("stime: " + std::to_string(stime));
            }
            else if(index == 22){
                starttime = std::stol(field);
                // std::cout << "starttime - " << starttime;
                LogMonitoring("starttime: " + std::to_string(starttime));
            }
            else if (index == idx){
                std::cout << idx <<" - ";
                DebugLog(field);
            }
        index++;
        }
        long total_cpu_ticks = utime + stime;
        LogMonitoring("Total CPU time: " +std::to_string(total_cpu_ticks) + " ticks");
    }
    return 0;
}

int main(){

    return 0;
}