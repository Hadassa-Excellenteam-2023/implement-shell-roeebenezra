#include <string>
#include <sstream>
#include <unistd.h>


// This class is used to resolve the path of a command.
class PathResolver {
public:
    static char *resolvePath(const std::string &command) {
        bool hasFullPath = (access(command.c_str(), X_OK) == 0);

        // If the command does not have a full path, then we need to search for it in the PATH environment variable
        if (!hasFullPath) {
            char *commandPath;
            std::string pathEnv = std::getenv("PATH");

            std::istringstream iss(pathEnv);
            std::string directory;

            // Iterate over each directory in the PATH environment variable
            while (std::getline(iss, directory, ':')) {
                std::string commandFullPath = directory += "/" + command;
                if (access(commandFullPath.c_str(), X_OK) == 0) {
                    commandPath = new char[commandFullPath.size() + 1];
                    std::strcpy(commandPath, commandFullPath.c_str());
                    return commandPath;
                }
            }
        }
            // If the command has a full path, then we can just return it
        else {
            char *commandPath = new char[command.size() + 1];
            std::strcpy(commandPath, command.c_str());
            return commandPath;
        }

        return nullptr;
    }
};