
#pragma once

#include <sstream>
#include <string>
#include <vector>


class InputParser {
public:

    // This method is used to parse the input from the user
    static void parseInput(const std::string &input, std::string &command, std::vector<std::string> &args) {
        std::istringstream iss(input);
        std::string token;

        if (!(iss >> command))
            return;

        // Parse the arguments
        while (iss >> token) {
            args.push_back(token);
        }
    }
};

