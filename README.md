# implement-shell-roeebenezra

## Custom Shell part 2

This is a custom shell program implemented in C++.
It functions similarly to the standard Linux shell, where it accepts commands and arguments from the user and executes
them as subprocesses.
The program utilizes various system calls and features of Linux to achieve its functionality.

## Features

Command Execution: The shell can execute commands and their arguments provided by the user.
It utilizes the fork and 'execv' system calls to create child processes and run the commands, respectively.

Command Path Resolution: The shell resolves the full path of a command by searching through directories listed in the
PATH environment variable. It checks for executability in each directory and returns the full path if found.

Background Processes: The shell supports running commands as background processes by appending the '&' symbol at the end
of a command. Background processes run concurrently with the shell, allowing users to continue entering commands while
the processes execute.

Background Process Monitoring: The shell tracks and monitors background processes. It provides a command, myjobs, to
display information about background processes, including their process IDs (PIDs), commands, and status.
Error Handling: The shell provides appropriate error messages for various scenarios, such as command not found or failed
execution.


## Limitations
The shell assumes a Unix/Linux environment and may not work on other operating systems.

It supports a limited set of commands and may not handle all possible scenarios or advanced shell features.

Error handling is basic and may not cover all edge cases.

