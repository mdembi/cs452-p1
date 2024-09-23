#include <stdio.h>
#include <unistd.h>
#include "lab.h"
#include <stdlib.h>
#include <string.h>


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
