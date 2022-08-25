#include "headers.h"

char message[2][100];


int verify(char *str) {
	
	int flag=0;
	
	char* token = strtok(str, "\n");
	while(token != NULL){
		strcpy(message[flag], token);
		//printf("%d",strlen(token));
		token = strtok(NULL, " ");
		flag++;
	}
	
	//printf("%d", strlen(message[0]));
	//printf("\n%d\n", strlen(message[1]));
	if((strcmp(message[0],"user") == 0) && (strcmp(message[1],"pass\n") == 0)){
		printf("User Logged In\n\n");
		return(1);
	}
	return(0);
}
	
	