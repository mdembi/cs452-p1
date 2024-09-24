#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>

// Function prototype for built-in command handler
void exit_shell(char **argv);

// Define a structure for built-in commands
struct command {
    char *name;
    void (*func)(char **);
};

// Declare the commands array as extern
extern struct command commands[];

#endif // COMMANDS_H
