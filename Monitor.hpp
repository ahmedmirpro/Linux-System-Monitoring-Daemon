/**
 * هادشي كيتسمّى include guard، والهدف ديالو هو يمنع نفس الـ header من أنه يتـinclude أكثر من مرة أثناء compilation.
 * if not def MONITOR_HPP, define MONITOR_HPP ==> it means the define will be here only, if detected by compiled somewhere else it won't compile
 */
#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <string>
#include <csignal>

// using namespace std;

class Monitor
{
private:
    /** private attributes - used in methods() or accessed by setters/getters */ 
    std::string proc_path; // build path for process of the PID
    std::string status_path;
    bool debug_enabled = false;
public:
    /* structures */
    struct ProcessCpuStats {
        long utime; // user time
        long stime; // system time
        long starttime; // to capture at the beginning of the runtime
        long total_cpu_ticks;
        double total_cpu_seconds;
        bool valid = false;
    };
    /* public attributes - used in main() */ 
    // int pid, interval;
    /* methods */
    bool ProcessExists(int pid);
    int DisplayProcess(const std::string& formatted_time); // il recup la valeur stockée deja en memoire en passant l'& de la valeur renvoyée par ReturnDatetime()
    void EnableDebug(bool enabled);
    void DebugLog(const std::string& message);
    std::string ReturnDatetime();
    void LogMonitoring(const std::string& ss); //il recup la valeur stockée deja en memoire en passant l'& de la valeur passé en param
    ProcessCpuStats ReadCpuStats(int pid);
    void LogCpuStats(const ProcessCpuStats& stats);
};

extern volatile sig_atomic_t keep_running; //explain what does extern do here (avoid multiple def maybe)
void sigint_handler(int sig);


#endif