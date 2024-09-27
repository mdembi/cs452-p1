#ifndef COMMANDS_H
#define COMMANDS_H

// Declare builtin commands
int change_dir(char **argv);
void print_history();
void execute_command(char **argv_cmd);
char *trim_white(char *line);

#endif