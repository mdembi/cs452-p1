#include <stdio.h>
#include "../src/lab.h"
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../src/commands.h"

int main(int argc, char **argv) 
{
    parse_args(argc, argv);

    // Set the prompt
    char *prompt = get_prompt("MY_PROMPT");

    // Free the the prompt memory
    free(prompt); 

    // Start checking additional inputs for built-in commands
    input_check();

    return 0;
}

void input_check() {
    char *line;
    char **argv;

    while (1) {
        char *prompt = get_prompt("MY_PROMPT");
        line = readline(prompt);
        free(prompt);

        // check for EOF
        if (line == NULL) { 
            printf("\n");
            break;
        }

        add_history(line); // basically lets us scroll via Readline
        argv = cmd_parse(line);

        if (argv != NULL && argv[0] != NULL) {
            int found = 0;

            // Check for built-in commands
            for (int i = 0; commands[i].name != NULL; i++) { // Check against placeholders
                if (strcmp(argv[0], commands[i].name) == 0) {
                    commands[i].func(argv);
                    found = 1;
                    break;
                }
            }

            if (!found) {
                // to deal with fork/exec that Shane warns about
            }
        }

        free(line);
        cmd_free(argv);
    }
}