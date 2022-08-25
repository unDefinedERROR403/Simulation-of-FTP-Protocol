#include "headers.h"

#include "user.c"
#include "pass.c"
#include "authentication.c"
#include "mkd.c"
#include "cwd.c"
#include "rmd.c"


int new_socket, count=0, i=0,n=0;
char message[2][100];
int main() {
	system("clear");
	printf("--------------------------\n");
	printf("----------SERVER----------\n");
	printf("--------------------------\n\n\n");
	
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	socklen_t address_size;
	char namedPipe[50], renamefrom[PATH_MAX];
	int sd,login=0, fileflagname=0;
	char buff[5000];
	
	if((sd = socket(AF_INET, SOCK_STREAM,0))<0) {
		printf("Could not create socket\n");
		exit(1);
	}
	memset(&server_address, '\0', sizeof(server_address));
	server_address.sin_family = AF_INET;             
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	printf("Socket created with IP-address: %s\n\n\n", inet_ntoa(server_address.sin_addr));
	
	if(bind(sd,(struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
		printf("Bind failed\n");
		close(sd);
		exit(1);
	}
	printf("Bind successful to port: %d\n", PORT);
	
	if(listen(sd,10) == -1) {
		printf("Listening failed");
		exit(1);
	}
	printf("Listening...\n\n");
	
	while(1) {
		char client_addr[100];
		int client_port;
		if((new_socket = accept(sd, (struct sockaddr*)&client_address, &address_size))<0) {
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));  
		strcpy(client_addr, inet_ntoa(client_address.sin_addr));
		client_port = ntohs(client_address.sin_port);
        printf("Client is not yet Logged In.\n\n\n");
		
		if (fork() == 0 ) {
			close(sd); 
		
		while(login == 0) {
			
			recv(new_socket, buff, MAXLINE, 0);
			login = verify(buff);
			if(login == 1){
				strcpy(buff,"0");
				send(new_socket, buff, sizeof(buff),0);
			}
			else {
				strcpy(buff,"Incorrect username or password ");
				send(new_socket, buff, sizeof(buff),0);
			}
		}
		
		bzero(buff, sizeof(buff));
        strcpy(buff, "\n\n");
            strcat(buff, "File Transfer Protocol Services\n\n");

            strcat(buff, "1.  USER\n");
            strcat(buff, "2.  PASS\n");
            strcat(buff, "3.  MKD\n");
            strcat(buff, "4.  CWD\n");
            strcat(buff, "5.  RMD\n"); 
            strcat(buff, "6.  PWD\n");
            strcat(buff, "7.  RETR\n");
            strcat(buff, "8.  STOR\n");
            strcat(buff, "9.  LIST\n" );
            strcat(buff, "10. ABOR\n");
            strcat(buff, "11. REIN\n");
			strcat(buff, "12. DELE\n");
			strcat(buff, "13. CDUP\n");
			strcat(buff, "14. QUIT\n");
			strcat(buff, "15. PORT\n");
			strcat(buff, "16. RNFR\n");
			strcat(buff, "17. RNTO\n");
            strcat(buff, "\n\n");
            send(new_socket, buff, MAXLINE,0);    
            bzero(buff, sizeof(buff));   
			memset(&client_address, '\0', sizeof(client_address));
			while(1){
				i=0;
				n=0;
				char temp[MAXLINE];
                *temp = '\0';
                bzero(buff, sizeof(buff));
                if(recv(new_socket, buff, MAXLINE, 0) < 0) {     
                    strcat(temp, "Data Connection Open but No Transfer in progress.");
                    send(new_socket, temp, MAXLINE, 0);
                    continue;
                }
				char* token = strtok(buff, " ");
				while(token != NULL){
					strcpy(message[i], token);
					token = strtok(NULL, " ");
					i++;
				}
				if(strcmp(message[0],"1")==0){
					printf("USER command\n");
					
					if( strncmp(message[1],"USER ",5) == 0  || strncmp(message[1],"user ",5) == 0  || strncmp(message[1],"USER\t",5) == 0 || strncmp(message[1],"user\t",5) == 0 || strncmp(message[1],"USER\n",5) == 0 || strncmp(message[1],"user\n",5) == 0  ) {
        	
						bzero(buff, sizeof(buff));  
						printf("Username: %s",message[1]);
						if(verify_user(message[1]) == 1){
							bzero(buff, sizeof(buff));
							strcpy(temp,"User Verified ");
							send(new_socket, temp, sizeof(temp),0);
						}
					}
				}
				
				if(strcmp(message[0],"2")==0){
					printf("PASS command\n");
					
					if( strncmp(message[1],"PASS ",5) == 0  || strncmp(message[1],"pass ",5) == 0  || strncmp(message[1],"PASS\t",5) == 0 || strncmp(message[1],"pass\t",5) == 0 || strncmp(message[1],"PASS\n",5) == 0 || strncmp(message[1],"pass\n",5) == 0  ) {
        	
						bzero(buff, sizeof(buff));   
						printf("Password: %s",message[1]);
						if(verify_pass(message[1]) == 1){
							bzero(buff, sizeof(buff));
							strcpy(temp,"User Logged In ");
							send(new_socket, temp, sizeof(temp),0);
						}
					}
				}
				
				
				if(strcmp(message[0],"3")==0){
					printf("MKD command\n");
					
					bzero(buff, sizeof(buff)); 
					if(make_directory(message[1]) == 1){
						bzero(buff, sizeof(buff));
						strcpy(temp,"Directory created ");
						printf("Directory created: %s\n",message[1]);
						send(new_socket, temp, sizeof(temp),0);
					}
					else {
						if(make_directory(message[1]) == -1){
						bzero(buff, sizeof(buff));
						strcpy(temp,"Directory already exixts ");
						send(new_socket, temp, sizeof(temp),0);
						}
						else{
							
						bzero(buff, sizeof(buff));
						strcpy(temp,"Can't create directory ");
						send(new_socket, temp, sizeof(temp),0);
						}
								
					}
				}
				
				if(strcmp(message[0],"4")==0){
					printf("CWD command\n");
					bzero(buff, sizeof(buff));
					if(change_directory(message[1]) == 0){
						bzero(buff, sizeof(buff));
						strcpy(temp,"Directory changed ");
						printf("Directory changed\n");
						send(new_socket, temp, sizeof(temp),0);
					}
					else {
						if(change_directory(message[1]) == -1){
						bzero(buff, sizeof(buff));
						strcpy(temp,"Permission denied ");
						send(new_socket, temp, sizeof(temp),0);
						}
						else{
							if(change_directory(message[1]) == 1){
							bzero(buff, sizeof(buff));
							strcpy(temp,"Invalid directory ");
							send(new_socket, temp, sizeof(temp),0);
							}
							else{
							bzero(buff, sizeof(buff));
							strcpy(temp,"Could not change directory ");
							send(new_socket, temp, sizeof(temp),0);
							}
						}
								
					}
				}
				
				if(strcmp(message[0],"5")==0){
					printf("RMD command\n");
					bzero(buff, sizeof(buff));
					if(remove_directory(message[1]) == 0){
						bzero(buff, sizeof(buff));
						strcpy(temp,"Directory removed ");
						printf("Directory removed\n");
						send(new_socket, temp, sizeof(temp),0);
					}
					else {
						if(remove_directory(message[1]) == -1){
						bzero(buff, sizeof(buff));
						strcpy(temp,"Permission denied/No directory found ");
						send(new_socket, temp, sizeof(temp),0);
						}
						else{
							if(remove_directory(message[1]) == 1){
							bzero(buff, sizeof(buff));
							strcpy(temp,"Invalid directory ");
							send(new_socket, temp, sizeof(temp),0);
							}
							else{
							bzero(buff, sizeof(buff));
							strcpy(temp,"Could not remove directory ");
							send(new_socket, temp, sizeof(temp),0);
							}
						}
								
					}
				}
				
				if(strcmp(message[0],"6")==0){
					printf("PWD command\n");
					bzero(buff, sizeof(buff)); 
					if(getcwd(buff, 256) == NULL) {
						strcpy(temp,"Error in getting current working directory ");
						send(new_socket, temp, sizeof(temp),0);
					}
					else {
						strcpy(temp,strtok(buff, "\n"));
						send(new_socket, temp, sizeof(temp),0);
					}
				}
				
				if(strcmp(message[0],"9")==0){
					printf("LIST command\n");
					bzero(buff, sizeof(buff)); 
					char list[1024];
					DIR * directory;
					struct dirent * entry;
					*list = '\0';

						if((directory = opendir("./")) == NULL) {
							strcpy(list, "Error while opening directory");
						}
							while((entry = readdir(directory)) != NULL) {
								if((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) {
									strcat(list, "-> ");
									strcat(list, entry->d_name);
									strcat(list,"\n");
						
									}	
								}
							closedir(directory);
					send(new_socket,list,MAXLINE,0);
				}
				
				if(strcmp(message[0],"7")==0){
					printf("RETR command\n");
					bzero(buff, sizeof(buff)); 
					char file[1024];
					strtok(message[1], "\n");
					char ch;
					FILE *fp1;
    
					if((fp1 = fopen(message[1], "r")) == NULL) {
						if(errno == ENOENT) {
							return 347;
						}
						return 348;
					}
    
					int k = 0;
					ch = fgetc(fp1);
					while (ch != EOF) {
						file[k] = ch;
						k++;
						ch = fgetc(fp1);
					}
					file[k] = '\0';
					
					
					send(new_socket,file,MAXLINE,0);
				}
				
				if(strcmp(message[0],"11")==0){
					printf("REIN command\n");
					bzero(buff, sizeof(buff));
					srand(time(NULL)); 
					int no = (rand()%2);
					if (login == 1){
						 printf("Connection closed from %s:%d\n", client_addr, client_port);
						 printf("Waiting for client to make connection\n\n");
						login = 0;
						break;
					}
				}
					
				if(strcmp(message[0],"10")==0){
					printf("ABOR command\n");
					bzero(buff, sizeof(buff));
					srand(time(NULL)); 
					int no = (rand()%2);
					if (login == 1){
						 printf("Error at client side\n\n");
						
					}
					bzero(buff, sizeof(buff));
					strcpy(buff, "\n\n");
					strcat(buff, "File Transfer Protocol Services\n\n");

					strcat(buff, "1.  USER\n");
					strcat(buff, "2.  PASS\n");
					strcat(buff, "3.  MKD\n");
					strcat(buff, "4.  CWD\n");
					strcat(buff, "5.  RMD\n");        
          
					strcat(buff, "6.  PWD\n");
					strcat(buff, "7.  RETR\n");
					strcat(buff, "8.  STOR\n");
					strcat(buff, "9.  LIST\n" );
					strcat(buff, "10. ABOR\n");
					strcat(buff, "11. REIN\n");
					strcat(buff, "12. DELE\n");
					strcat(buff, "13. CDUP\n");
					strcat(buff, "14. QUIT\n");
					strcat(buff, "15. PORT\n");
					strcat(buff, "16. RNFR\n");
					strcat(buff, "17. RNTO\n");
					strcat(buff, "\n\n");
					send(new_socket, buff, MAXLINE,0);
				}
				
				if(strcmp(message[0],"12")==0){
					printf("DELE command\n");
					bzero(buff, sizeof(buff));
					if(remove_directory(message[1]) == 0){
						bzero(buff, sizeof(buff));
						strcpy(temp,"File deleted ");
						printf("File: %sStatus: deleted\n",message[1]);
						send(new_socket, temp, sizeof(temp),0);
					}
					else {
						if(remove_directory(message[1]) == -1){
						bzero(buff, sizeof(buff));
						strcpy(temp,"Permission denied/No file found ");
						send(new_socket, temp, sizeof(temp),0);
						}
						else{
							if(remove_directory(message[1]) == 1){
							bzero(buff, sizeof(buff));
							strcpy(temp,"Invalid file ");
							send(new_socket, temp, sizeof(temp),0);
							}
							else{
							bzero(buff, sizeof(buff));
							strcpy(temp,"Could not delete file ");
							send(new_socket, temp, sizeof(temp),0);
							}
						}
								
					}
				}
				
				if(strcmp(message[0],"13")==0){
					printf("CDUP command\n");
					
					if(chdir("..")==0){
						bzero(buff, sizeof(buff));
						strcpy(buff, "Working directory changed ");
						send(new_socket, buff, sizeof(buff),0);
					}
					else {
						bzero(buff, sizeof(buff));
						strcpy(buff, "Error while changing working directory ");
						send(new_socket, buff, sizeof(buff),0);
					}
						
				}
				
				if(strcmp(message[0],"14")==0){
					printf("QUIT command\n");
					printf("Server closed");
					close(new_socket);
					close(sd);
					login=1;
					kill(0, SIGINT);
					kill(0, SIGKILL);
					exit(1);
					break;
				}
				
				if(strcmp(message[0],"15")==0){
					printf("PORT command\n");
					
					strcpy(namedPipe, "/tmp/");
                
					strcat(namedPipe, message[1]);
					unlink(namedPipe);
					if (mkfifo(namedPipe, 0777) != 0)
					{
						bzero(buff, sizeof(buff));
						strcpy(buff, "Error while opening data connection ");
						send(new_socket, buff, sizeof(buff),0);
						continue;
					}
					chmod("namedPipe", 0777);
					bzero(buff, sizeof(buff));
					printf("Transfering through named pipe: %s", message[1]);
					strcpy(buff, "Transfer through named pipe ");
					send(new_socket, buff, sizeof(buff),0);
					
				}
				
				if(strcmp(message[0],"16")==0){
					printf("RNFR command\n");
					
					strcpy(renamefrom, message[1]);
					fileflagname =1;
					bzero(buff, sizeof(buff));
					printf("File name to be changed: %s", message[1]);
					strcpy(buff, "Copied the file name ");
					send(new_socket, buff, sizeof(buff),0);
					
				}
				
				if(strcmp(message[0],"17")==0){
					printf("RNTO command\n");
					
					if(fileflagname == 0) {
						bzero(buff, sizeof(buff));
						strcpy(buff, "Error: enter file name through RNFR ");
						send(new_socket, buff, sizeof(buff),0);
					}
					else {
						char renameto[100];
						strcpy(renameto, message[1]);
						strtok(renamefrom, "\n");
						strtok(renameto, "\n");
						if(rename(renamefrom, renameto)==0){
						
							printf("Old file name: %s\n", renamefrom);
							printf("New file name: %s\n", message[1]);
							bzero(buff, sizeof(buff));
							strcpy(buff, "File name changed ");
							send(new_socket, buff, sizeof(buff),0);
						}
						else{
							printf("error");
						}
					}
					
				}
				
				//break;
			}
		}
	}
		close(new_socket);
		return(0);
}
	
	