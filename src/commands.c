#include <stdio.h>
#include "../src/lab.h"
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <pwd.h>
#include "commands.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <limits.h>
#include <ctype.h>
#include <signal.h>

/**
 * Built-in function for user to change directories
*/
int change_dir(char **argv) { 
    const char *path;

    // Default to home if no option is given
    if (argv[1] == NULL) {
        // First try via getenv
        path = getenv("HOME");
        // If getevn is null, use getuid and getpwuid
        if (path == NULL) {
            struct passwd *pw = getpwuid(getuid());
            if (pw == NULL || pw->pw_dir == NULL) {
                fprintf(stderr, "I can't seem to find your home directory...\n");
                return 1; 
            }
            path = pw->pw_dir;
        }
    } else {
        path = argv[1];
    }

    // Error if directory can't be changed
    if (chdir(path) != 0) {
        fprintf(stderr, "I can't seem to find that directory...\n");
        return -1; 
    }

    return 0; 
}

/**
 * Built-in function to print command history
*/
void print_history() {
    HIST_ENTRY **hist_entries = history_list();
    
    if (hist_entries == NULL) {
        printf("I can't seem to find a history...\n");
        return;
    }

    for (int i = 0; hist_entries[i] != NULL; i++) {
        printf("%d  %s\n", i + 1, hist_entries[i]->line);
    }
}

/**
 * Function to execute a command using fork and execvp
 */
void execute_command(char **argv_cmd) {
    // Start with fork
    pid_t pid = fork();

    //If returns -1, that indicates an error
    if (pid == -1) {
        perror("An error occurred");
        return;
    } else if (pid == 0) {
        // Set signals to default in child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);

        // Execute the command
        execvp(argv_cmd[0], argv_cmd);
        
        // If execvp returns, an error occurred
        perror("An error occured");
        exit(1);
    } else {
        // Wait for child process to finish
        int status;
        waitpid(pid, &status, 0);
    }
}

/**
 * Function to trim whitespace
*/
char* trim_white(char *str) {
    if (str == NULL) return NULL;

    // Move the pointer to the first non-whitespace character
    while (isspace((unsigned char)*str)) str++;

    // If the string is all whitespace, return an empty string
    if (*str == '\0') return str;

    // Find the end of the string
    char *end = str + strlen(str) - 1;

    // Move the pointer back to the last non-whitespace character
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null-terminate the string after the last non-whitespace character
    *(end + 1) = '\0';

    return str;
}

/**
 * Here's a lovely list of references:
 * https://stackoverflow.com/questions/2552416/how-can-i-find-the-users-home-dir-in-a-cross-platform-manner-using-c
 * https://stackoverflow.com/questions/2910377/get-home-directory-in-linux
 * https://www.math.utah.edu/docs/info/hist_2.html
 * https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
 * https://stackoverflow.com/questions/49122665/how-to-use-execvp-to-execute-a-command
 * https://www.digitalocean.com/community/tutorials/how-to-use-bash-s-job-control-to-manage-foreground-and-background-processes
 * 
*/