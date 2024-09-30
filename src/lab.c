#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"
#include <signal.h>

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

/**
 * Shell function to also handle signals 
*/
void sh_init(struct shell *sh) {
    // Initialize shell structure
    sh->shell_terminal = STDIN_FILENO;
    sh->shell_pgid = getpid();
    sh->shell_is_interactive = isatty(sh->shell_terminal);
    
    // Put the shell in its own process group
    setpgid(sh->shell_pgid, sh->shell_pgid);
    
    // Ignore signals in the shell (parent process)
    signal(SIGINT, SIG_IGN);    // Ignore interrupt signal (Ctrl+C)
    signal(SIGQUIT, SIG_IGN);   // Ignore quit signal (Ctrl+\)
    signal(SIGTSTP, SIG_IGN);   // Ignore stop signal (Ctrl+Z)
    signal(SIGTTIN, SIG_IGN);   // Ignore terminal read signals
    signal(SIGTTOU, SIG_IGN);   // Ignore terminal write signals

    // Change the foreground process group
    tcsetpgrp(sh->shell_terminal, sh->shell_pgid);
}