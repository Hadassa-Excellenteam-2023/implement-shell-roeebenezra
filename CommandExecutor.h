
#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <map>

#include "PathResolver.h"


class CommandExecutor {
public:

    // This method is used to execute a command with the given arguments
    void executeCommand(const std::string &command, const std::vector<std::string> &args, bool isBackground) {
        pid_t pid = fork();

        // Fork returns -1 if an error occurred
        if (pid == -1) {
            std::cerr << "Failed to fork a new process." << std::endl;
            return;
            // Fork returns 0 if it is the child process
        } else if (pid == 0) {
            // Child process
            std::vector<char *> cArgs;
            cArgs.reserve(args.size() + 2);
            cArgs.push_back(const_cast<char *>(command.c_str()));  // First argument is the command itself

            // Convert the vector of strings to a vector of char *
            for (const auto &arg: args) {
                cArgs.push_back(const_cast<char *>(arg.c_str()));
            }

            cArgs.push_back(nullptr);  // Last argument must be nullptr

            char *commandPath = PathResolver::resolvePath(command);

            // If the commandPath is nullptr, then the command was not found
            if (commandPath == nullptr) {
                std::cerr << "Command not found: " << command << std::endl;
                exit(EXIT_FAILURE);
            }

            execv(commandPath, cArgs.data());

            // execv returns only if an error occurred
            std::cerr << "Failed to execute the command: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            if (!isBackground) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                backgroundProcesses[pid] = command;
            }
        }
    }

    // This method is used to print the background processes
    void printBackgroundProcesses() {
        std::cout << "Background Processes:" << std::endl;

        for (const auto &process: backgroundProcesses) {
            pid_t pid = process.first;
            const std::string &command = process.second;

            std::cout << "PID: " << pid << ", Command: " << command << std::endl;
        }
    }

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

