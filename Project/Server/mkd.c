#include "headers.h"

struct stat st = {0};

int make_directory(char *directory) {


    strtok(directory, "\n");
    int status = stat(directory, &st);
    
    if (status == -1) {
        mkdir(directory, 0777);
        return 1;
    }

    if (status == 0) 
        return -1;

    return 0;
}