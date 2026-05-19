#include <iostream>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
    if(argc !=2){ // make sure the user did not type more than one argument
        std::cerr << "Usage: ./monitor <pid>\n";
        return 1;
    }
    int pid = std::stoi(argv[1]); // convert the second arg from char to int (to be easily manipulated)

    if (pid <= 0){ // data entry sanity check
        std::cerr << "Invalid PID\n" << std::endl;
        return 1;
    }

    std::string proc_path = "/proc/" + std::to_string(pid); // build the process path
    std::cout << "Checking path: " << proc_path << std::endl; // debug log to make sure the pid exists and print it in the terminal

    // if(std::filesystem::exists(proc_path)){ // check if the path built exists in the filesystem
    if(access(proc_path.c_str(), F_OK) == 0){ // access(proc_path.c_str(), F_OK) checks whether the path stored in the std::string proc_path exists in the filesystem by converting it to a const char*; it returns 0 if the file/directory exists and -1 otherwise.
        std::cerr << "PID exists\n";
        std::string status_path = proc_path + "/status";
        std::ifstream file(status_path);
        if(!file.is_open()){
            std::cerr << "Failed to open the file Status\n";
            return 1;
        }
        std::string line; //pid_name
        std::cerr << "PID Info:\n";
        while(std::getline(file, line)){ //pid_name // std::getline(file, line) كتقرا سطر واحد من الملف وتحطو فـ line، وifstream كيبقى حافظ داخلياً على position ديال القراءة (cursor)، لذلك كل مرة كتعاود تتنادى داخل while كتقرا automatiquement السطر اللي من بعد حتى توصل لنهاية الملف، وفي هاد اللحظة getline() كترجع false وكتوقف loop.
            if(line.rfind("Name:", 0) == 0 ||
               line.rfind("State:", 0) == 0 ||
               line.rfind("Pid:", 0) == 0 ||
               line.rfind("PPid:", 0) == 0 ||
               line.rfind("VmRSS:", 0) == 0)
            {
                std::cout << line << std::endl;
            }
        }
    }
    else
        std::cerr << "PID not found!\n";
    return 0;
}