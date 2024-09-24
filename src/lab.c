#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"

// Function prototypes for internal use
char **cmd_parse(const char *line);
void cmd_free(char **line);

void parse_args(int argc, char **argv) {
    int opt;
    
    // Parse command line
    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                // Print version and exit
                printf("Version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
                exit(0);
            default:
                exit(1);
        }
    }
}

/*
 * Define get_prompt 
 */
char *get_prompt(const char *env) {
    // Get the environment variable
    const char *prompt = getenv(env);

    // if prompt is not NULL, return it
    if (prompt != NULL) {
        return strdup(prompt);
    }
    // Return the default prompt if MY_PROMPT is not set
    return strdup("type_here>");
}

/*
 * Command parsing implementation
 */
char **cmd_parse(const char *line) {
    char **argv = malloc(64 * sizeof(char *)); // Allocate memory for argument array
    if (!argv) return NULL; // Check allocation success

    unsigned int index = 0;
    char *token = strtok(strdup(line), " "); // Duplicate the line to avoid modifying the original
    while (token != NULL) {
        argv[index++] = token;
        token = strtok(NULL, " ");
    }
    argv[index] = NULL; // Null-terminate the array

    return argv;
}

/*
 * Free the memory allocated for the command arguments
 */
void cmd_free(char **line) {
    if (line) {
        for (unsigned int i = 0; line[i] != NULL; i++) {
            free(line[i]); // Free each token
        }
        free(line); // Free the argument array
    }
}
