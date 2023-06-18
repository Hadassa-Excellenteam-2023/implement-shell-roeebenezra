
#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <map>
#include <sys/wait.h>
#include <fcntl.h>

#include "PathResolver.h"


class CommandExecutor {
public:
    // default constructor
    CommandExecutor() = default;

    // This method is used to execute a command with the given arguments
    void executeCommand(const std::string &command, const std::vector<std::string> &args, bool isBackground,
                        const std::string &inputFile = "", const std::string &outputFile = "",
                        int inputPipe = -1, int outputPipe = -1);

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

    // Helper method to handle input and output redirection
    static void handleRedirection(const std::string &inputFile, const std::string &outputFile);

    // Helper method to handle pipe creation and connection
    static void handlePipes(int inputPipe, int outputPipe);
};

