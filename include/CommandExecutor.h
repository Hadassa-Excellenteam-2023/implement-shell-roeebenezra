
#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <map>

#include "PathResolver.h"


class CommandExecutor {
public:
    // default constructor
    CommandExecutor() = default;

    // This method is used to execute a command with the given arguments
    void executeCommand(const std::string &command, const std::vector<std::string> &args, bool isBackground);

    // This method is used to print the background processes
    void printBackgroundProcesses();

    // This method is used to check if a process is a background process
    bool isBackgroundProcess(pid_t pid) {
        return backgroundProcesses.find(pid) != backgroundProcesses.end();
    }

    // This method is used to remove a background process
    void removeBackgroundProcess(pid_t pid) {
        backgroundProcesses.erase(pid);
    }


private:
    std::map<pid_t, std::string> backgroundProcesses;
};

