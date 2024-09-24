#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../src/lab.h"

// Function prototypes
void exit_shell(char **argv);

// Define a structure for built-in commands
struct command {
    char *name;
    void (*func)(char **);
};

// Array of built-in commands
struct command commands[] =  {
    {"exit", exit_shell},
    {NULL, NULL}  // Placeholder value for easier iteration
};

// Function to handle the exit command
void exit_shell(char **argv) {
    if (argv[1] != NULL) {
        fprintf(stderr, "exit: too many arguments\n");
        return;
    }
    // Exit with return value 0
    exit(0);
}
