#include "headers.h"

char user_name[2][100] = {"USER","user"};
//bool user_verified = false;


int verify_user(char *username) {
		
		for (int i = 0; i < 2; i++) {
        if (strncmp(username,user_name[i],4) == 0) {
            //user_verified = true;
            count = i;
			printf("\nUser verified\n\n");
			return(1);
        }
    }
	return(0);
   // return(user_verified); 
}

