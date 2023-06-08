
#pragma once

#include <iostream>
#include <vector>
#include <sys/wait.h>

#include "CommandExecutor.h"
#include "InputParser.h"


class Shell {
public:
    // This method is used to run the shell in an infinite loop
    void run() {
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

private:
    CommandExecutor commandExecutor;
};