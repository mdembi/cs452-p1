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
    return strdup("shell>");
}

/*
 * Command parsing implementation
 */
char **cmd_parse(const char *line) {
    // Allocate memory for argument array
    char **argv = malloc(64 * sizeof(char *));
    // Check to see if memory was correctly allocated
    if (!argv) return NULL; 

    unsigned int index = 0;
    // Copy the line
    char *line_copy = strdup(line); 
    // Use the copy and tokenize
    char *token = strtok(line_copy, " "); 

    while (token != NULL) {
        // Allocate memory for each token
        argv[index] = strdup(token);
        index++;
        token = strtok(NULL, " ");
    }
    // Null out the array
    argv[index] = NULL;

    // Free the copy
    free(line_copy); 
    return argv;
}

/*
 * Free the memory allocated for the command arguments
 */
void cmd_free(char **line) {
    if (line) {
        // Free each token
        for (unsigned int i = 0; line[i] != NULL; i++) {
            free(line[i]); 
        }
        // Free the line
        free(line); 
    }
}
