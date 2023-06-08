
#include "Shell.h"


void Shell::run() {
    while (true) {
        std::string input;
        std::cout << "myshell> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        std::string command;
        std::vector<std::string> args;
        InputParser::parseInput(input, command, args);

        // Check if the command is "myjobs"
        if (command == "myjobs") {
            commandExecutor.printBackgroundProcesses();
        } else {
            commandExecutor.executeCommand(command, args, false);
        }

        // Check if there are any background processes that have completed
        int status;
        pid_t pid;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            if (commandExecutor.isBackgroundProcess(pid)) {
                commandExecutor.removeBackgroundProcess(pid);
            }
        }
    }
}