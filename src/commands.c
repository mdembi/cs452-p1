#include <stdio.h>
#include "../src/lab.h"
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <pwd.h>
#include "commands.h"

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