#include "headers.h"

char user_password[2][100] = {"pass","PASS"};

int verify_pass(char *password) {
		
		for (int i = 0; i < 2; i++) {
        if (strncmp(password,user_password[i],4) == 0) {
            //user_verified = true;
            count = i;
			printf("\nLogged In\n\n");
			return(1);
        }
    }
	return(0);
   // return(user_verified); 
}