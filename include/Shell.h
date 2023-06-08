
#pragma once

#include <iostream>
#include <vector>
#include <sys/wait.h>

#include "CommandExecutor.h"
#include "InputParser.h"


class Shell {
public:
    // This method is used to run the shell in an infinite loop
    void run();

private:
    CommandExecutor commandExecutor;
};