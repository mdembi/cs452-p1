#include <stdio.h>
#include <unistd.h>
#include "lab.h"


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

