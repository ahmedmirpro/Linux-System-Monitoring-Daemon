#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <string>
#include <csignal>

// using namespace std;

class Monitor
{
private:
    /** private attributes - used in methods() or accessed by setters/getters */ 
    std::string proc_path;
    std::string status_path;
    bool debug_enabled = false;
public:
    /* public attributes - used in main() */ 
    // int pid, interval;
    /* methods */
    bool ProcessExists(int pid);
    int DisplayProcess(const std::string& formatted_time); // il recup la valeur stockée deja en memoire en passant l'& de la valeur renvoyée par ReturnDatetime()
    void EnableDebug(bool enabled);
    void DebugLog(const std::string& message);
    std::string ReturnDatetime();
    void LogMonitoring(const std::string& ss); //il recup la valeur stockée deja en memoire en passant l'& de la valeur passé en param
    int StatPID(int pid);

};

extern volatile sig_atomic_t keep_running; //explain what does extern do here (avoid multiple def maybe)
void sigint_handler(int sig);


#endif