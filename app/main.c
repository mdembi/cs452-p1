#include <stdio.h>
#include "../src/lab.h"
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

int main(int argc, char **argv) 
{
    // Parse arguments
    parse_args(argc, argv);

    // Define variables for user lines and command arguments
    char *line;
    char **argv_cmd;

    // Main loop to read user input
    while (1) {
        // Check for custom prompt
        char *prompt = get_prompt("MY_PROMPT");
        line = readline(prompt);
        free(prompt);

        // Check for EOF
        if (line == NULL) { 
            printf("\n");
            break;
        }

        // Add line to history
        add_history(line);

        // Parse the line and assign to argv_cmd
        argv_cmd = cmd_parse(line);

        // Check for built-in commands
        if (argv_cmd != NULL && argv_cmd[0] != NULL) { 
            int found = 0; 

            // Exit
            if (strcmp(argv_cmd[0], "exit") == 0) {
                exit_shell(argv_cmd);
                found = 1;
            } 

            // Placeholder for fork/exec Shane warns about
            if (!found) {
                
            }
        }

        // Free memory
        free(line); 
        cmd_free(argv_cmd);
    }

    return 0; 
}

/**
 * Built-in function for user to exit shell
*/
void exit_shell(char **argv) {
    if (argv[1] != NULL) {
        fprintf("You've added too many arguments!\n");
        return;
    }
    exit(0);
}