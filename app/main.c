#include <stdio.h>
#include "../src/lab.h"
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <pwd.h>
#include "../src/commands.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <limits.h>

// Declare functions to avoid errors
void exit_shell(char **argv);
void check_background_jobs();

#define MAX_JOBS 100

// Job structure to hold background process details
struct job {
    pid_t pid;
    char *command;
    int job_number;
    int completed; // 1 if completed, 0 otherwise
};

struct job background_jobs[MAX_JOBS];
int job_count = 0;

int main(int argc, char **argv) 
{
    struct shell sh;
    
    // Initialize shell
    sh_init(&sh);

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

        // Trim whitespace
        line = trim_white(line);

        // Check for background process
        int background_check = 0;
        char *trimmed_line = line;

        // Check for '&' at the end (allowing for whitespace)
        while (*trimmed_line && isspace((unsigned char)*trimmed_line)) trimmed_line++; // Skip leading spaces
        if (strlen(trimmed_line) > 0 && trimmed_line[strlen(trimmed_line) - 1] == '&') {
            background_check = 1;
            trimmed_line[strlen(trimmed_line) - 1] = '\0'; // Remove the '&' otherwise there will be issues
            trimmed_line = trim_white(trimmed_line); // Trim trailing spaces
        }

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

            // Change directory
            if (strcmp(argv_cmd[0], "cd") == 0) {
                if (change_dir(argv_cmd) != 0) {
                    fprintf(stderr, "Error changing directory\n");
                }
                found = 1;
            }

            // Print command history
            if (strcmp(argv_cmd[0], "history") == 0) {
                print_history();
                found = 1;
            }

            // Handle background job execution
            if (!found) {
                if (background_check) {
                    pid_t pid = fork();
                    if (pid == 0) {
                        // Child process
                        execvp(argv_cmd[0], argv_cmd);
                        perror("Command execution failed");
                        exit(1);
                    } else if (pid > 0) {
                        // Parent process
                        // Store the job
                        background_jobs[job_count].pid = pid;
                        background_jobs[job_count].command = strdup(trimmed_line);
                        background_jobs[job_count].job_number = job_count + 1; // Job numbers start from 1
                        background_jobs[job_count].completed = 0;
                        job_count++;
                        printf("[%d] %d %s\n", job_count, pid, trimmed_line);
                    }
                }
            // Execute external commands
            } else {
                execute_command(argv_cmd);
            }
        }

        // Check for completed background jobs
        check_background_jobs();

        // Free memory
        free(line); 
        cmd_free(argv_cmd);
    }

    return 0; 
}

/**
 * Check to see if background job are completed
 */
void check_background_jobs() {
    for (int i = 0; i < job_count; i++) {
        if (!background_jobs[i].completed) {
            int status;
            pid_t result = waitpid(background_jobs[i].pid, &status, WNOHANG);
            if (result == -1) {
                perror("waitpid error");
            } else if (result == 0) {
                // Process is still running
                continue;
            } else {
                // Process has completed
                background_jobs[i].completed = 1;
                printf("[%d] Done %s\n", background_jobs[i].job_number, background_jobs[i].command);
                free(background_jobs[i].command); // Free the command string
            }
        }
    }
}


/**
 * Built-in function for user to exit shell
*/
void exit_shell(char **argv) {
    if (argv[1] != NULL) {
        fprintf(stderr, "You've added too many arguments!\n");
        return;
    }
    exit(0);
}