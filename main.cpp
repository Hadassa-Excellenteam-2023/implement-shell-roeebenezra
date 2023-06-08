#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void executeCommand(const std::string& command) {
    std::vector<std::string> args;
    size_t pos = 0;
    size_t spacePos;

    spacePos = command.find(' ', pos);
    std::string program = command.substr(pos, spacePos - pos);
    pos = spacePos + 1;

    while ((spacePos = command.find(' ', pos)) != std::string::npos) {
        std::string arg = command.substr(pos, spacePos - pos);
        args.push_back(arg);
        pos = spacePos + 1;
    }

    std::string arg = command.substr(pos);
    args.push_back(arg);

    std::vector<char*> cArgs;
    cArgs.reserve(args.size() + 1);
    for (const auto& argument : args) {
        cArgs.push_back(const_cast<char*>(argument.c_str()));
    }
    cArgs.push_back(nullptr);

    std::string myPath = "/usr/bin:/bin"; // TODO: read from environment variable PATH
    bool found = false;
    size_t pathPos = 0;
    while ((spacePos = myPath.find(':', pathPos)) != std::string::npos) {
        std::string path = myPath.substr(pathPos, spacePos - pathPos);
        std::string fullPath = path += "/" + program;
        if (access(fullPath.c_str(), X_OK) == 0) {
            found = true;
            break;
        }
        pathPos = spacePos + 1;
    }

    if (!found) {
        std::cerr << "Command not found: " << program << std::endl;
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "Failed to fork!" << std::endl;
        return;
    } else if (pid == 0) {
        // child process executes the command
        execvp(program.c_str(), cArgs.data());
        std::cerr << "Failed to execute " << program << std::endl;
        exit(1);
    } else {
        // father process waits for child process to finish
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            int exitCode = WEXITSTATUS(status);
            std::cout << "Child process exited with status: " << exitCode << std::endl;
        } else if (WIFSIGNALED(status)) {
            int signal = WTERMSIG(status);
            std::cout << "Child process terminated with signal: " << signal << std::endl;
        }
    }
}


int main() {
    while (true) {
        std::string command;
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        } else if (command == "myJobs") {
            //
            continue;
        }

        bool runInBackground = false;
        if (!command.empty() && command.back() == '&') {
            command.pop_back();
            runInBackground = true;
        }

        executeCommand(command);

        if (runInBackground) {
            //
        }
    }

    return EXIT_SUCCESS;
}
