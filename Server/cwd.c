#include "headers.h"

int change_directory(char *directory) {

    strtok(directory, "\n");
    if(chdir(directory) == -1) {
        if(errno == EACCES) 
            return -1;
        else if(errno == ENOTDIR || errno == ENOENT) 
            return 1;
        
        else 
            return 2;
    }
    return 0;
}