#include <stdio.h>
#include "../src/lab.h"

int main(int argc, char **argv) 
{
    parse_args(argc, argv);

    // Set the prompt
    char *prompt = get_prompt("MY_PROMPT");

    // Print the prompt (for testing--can delete later)
    printf("%s\n", prompt);

    // Free the the prompt memory
    free(prompt); 

    return 0;
}
