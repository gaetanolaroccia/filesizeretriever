#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h> 
#include <malloc.h>
#include <dirent.h>
#include "itoa.c"
#define PORT 8088

int main(){
	//socket stuff
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8088
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8088
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
	{
        	perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
        while(1){
		//variables 
		char appoggio[1024]={0};
		char buffer[1024] = {0};
		char alength[1024]={0};
 		char buffer2[1024] = {0};
		char buff[1024] = {0};
		char * welcome = "Welcome to Remote File Size Retriever, Please Insert File Path:\n";
                char * insert_length = "\nInsert File Path Character Length\n";
                char * come_back= "\nPlease, Feel Free to Come Back Again!\nRemember to Visit Us at https://github.com/gaetanolaroccia/filesizeretriever";
		char *fnf = "\nFile not Found\n";
		char *ff = "\nFile Found!\n";
		char * numero_caratteri = "\nCharacters Number: ";
		char * sender; 
    		FILE * fp;
		int res=0;
		int length =0;
		//accept to the socket		
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
			perror("accept");
        		continue;
		}
	
	
		//introduction
		res = send(new_socket,welcome,strlen(welcome),0);
		if(res<0){
			perror("send");
			close(new_socket);
			continue;
		}
		//file path
		res= recv(new_socket, buffer, 1023,0);
		if(res<0){
                        perror("receive");
                        close(new_socket);
                        continue;
                }
		
		//control over the buffer
		res = strlen(buffer);
		
		if (res <= 0 || res > 1024){
			perror("too big");
			close(new_socket);
			continue;
		}
		//suspicious character removal
		for(int j=0;j<strlen(buffer)-1;j++){
		        appoggio[j]=buffer[j];
        	}
		//fopen with controls
		fp = fopen(appoggio,"r");
                if(fp == NULL){
                        send(new_socket, fnf, strlen(fnf),0);
                        perror("File not Found");
			close(new_socket);
                        continue;
                }
		if(opendir(appoggio) != NULL){
		send(new_socket, fnf, strlen(fnf),0);
                        perror("File is a Directory");
                        close(new_socket);
                        continue;
		}

		//file found
		 res = send(new_socket, ff, strlen(ff),0);
                if(res<0){
                        perror("send");
                        close(new_socket);
                        continue;
                }

		//number of characters in path name
		res = send(new_socket,insert_length,strlen(insert_length),0);
		if(res<0){
                        perror("send");
                        close(new_socket);
                        continue;
                }
		res=recv(new_socket,buffer2,1023,0);
		if(res<0){
                        perror("receive");
                        close(new_socket);
                        continue;
                }
		
		for(int j=0;j<strlen(buffer2)-1;j++){
        	        alength[j]=buffer2[j];
	         }
		length = atoi (alength);
		if(length > 1024){
			length=1024;
		}
		if(length<0){
			length = 0;
		}
		printf("\n%s\n",appoggio);
			
		//fread
		fread(&appoggio[strlen(appoggio)],sizeof(char), sizeof(appoggio),fp);
		for(int j=0; j<length; j++){
			buff[j]=appoggio[j];
		}
		
		res = send(new_socket, buff, strlen(buff),0);
		if(res<0){
                        perror("send");
                        close(new_socket);
                        continue;
                }
		res = send(new_socket,numero_caratteri,strlen(numero_caratteri),0);
		if(res<0){
                        perror("send");
                        close(new_socket);
                        continue;
                }
		fseek(fp, 0L, SEEK_END);

    // calculating the size of the file
                res = ftell(fp);
                sender = (char*)malloc(1024*sizeof(char));
                itoa (res, sender);
                res = send(new_socket, sender, strlen(sender),0);
		if(res<0){
                        perror("send");
                        close(new_socket);
                        continue;
                }

                free(sender);
        	fclose(fp);
		//outro
		res = send(new_socket, come_back, strlen(come_back),0);
        	if(res<0){
                        perror("send");
                        close(new_socket);
                        continue;
                }

		close(new_socket);
	} 

	return 0;



}







