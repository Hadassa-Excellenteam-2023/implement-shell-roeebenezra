#include "CommandExecutor.h"

void CommandExecutor::executeCommand(const std::string &command, const std::vector<std::string> &args,
                                     bool isBackground, const std::string &inputFile, const std::string &outputFile,
                                     int inputPipe, int outputPipe) {
    pid_t pid = fork();

    // Fork returns -1 if an error occurred
    if (pid == -1) {
        std::cerr << "Failed to fork a new process." << std::endl;
        return;
    }
        // Fork returns 0 if it is the child process
    else if (pid == 0) {
        // Child process
        handleRedirection(inputFile, outputFile);
        handlePipes(inputPipe, outputPipe);

        std::vector<char *> cArgs;
        cArgs.reserve(args.size() + 2);
        cArgs.push_back(const_cast<char *>(command.c_str())); // First argument is the command itself

        // Convert the vector of strings to a vector of char *
        for (const auto &arg: args) {
            cArgs.push_back(const_cast<char *>(arg.c_str()));
        }

        cArgs.push_back(nullptr); // Last argument must be nullptr

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

void CommandExecutor::printBackgroundProcesses() {
    std::cout << "Background Processes:" << std::endl;

    for (const auto &process: backgroundProcesses) {
        pid_t pid = process.first;
        const std::string &command = process.second;

        std::cout << "PID: " << pid << ", Command: " << command << std::endl;
    }
}

void CommandExecutor::handleRedirection(const std::string &inputFile, const std::string &outputFile) {
    // Input redirection
    if (!inputFile.empty()) {
        int inputFileDescriptor = open(inputFile.c_str(), O_RDONLY);
        if (inputFileDescriptor == -1) {
            std::cerr << "Failed to open input file: " << inputFile << std::endl;
            exit(EXIT_FAILURE);
        }
        if (dup2(inputFileDescriptor, STDIN_FILENO) == -1) {
            std::cerr << "Failed to redirect input file descriptor." << std::endl;
            exit(EXIT_FAILURE);
        }
        close(inputFileDescriptor);
    }

    // Output redirection
    if (!outputFile.empty()) {
        int outputFileDescriptor = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outputFileDescriptor == -1) {
            std::cerr << "Failed to open output file: " << outputFile << std::endl;
            exit(EXIT_FAILURE);
        }
        if (dup2(outputFileDescriptor, STDOUT_FILENO) == -1) {
            std::cerr << "Failed to redirect output file descriptor." << std::endl;
            exit(EXIT_FAILURE);
        }
        close(outputFileDescriptor);
    }
}

void CommandExecutor::handlePipes(int inputPipe, int outputPipe) {
    // Input pipe
    if (inputPipe != -1) {
        if (dup2(inputPipe, STDIN_FILENO) == -1) {
            std::cerr << "Failed to redirect input pipe." << std::endl;
            exit(EXIT_FAILURE);
        }
        close(inputPipe);
    }

    // Output pipe
    if (outputPipe != -1) {
        if (dup2(outputPipe, STDOUT_FILENO) == -1) {
            std::cerr << "Failed to redirect output pipe." << std::endl;
            exit(EXIT_FAILURE);
        }
        close(outputPipe);
    }
}