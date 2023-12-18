/*
 * EX3.c
 *
 *  Created on: 18 déc. 2023
 *      Author: ensea
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define buffsize 128
#define PORT "69"
const char * nl = "\n$ ";
const char * separators = " ";

int main(void){

	char buffer[buffsize];
	char * host;
	char * file;
	ssize_t ret;

	if(write(STDOUT_FILENO, nl, strlen(nl)) == -1){perror("write");exit(EXIT_FAILURE);}

	while(1){   // REPL = read-eval-print-loop
			while((ret = read(STDIN_FILENO, buffer, buffsize)) > 0){ //reading the command input
				buffer[strcspn(buffer, "\n")] = '\0';

				char * strToken = strtok(buffer, separators);
				while (strToken != NULL){

					if((strcmp(strToken, "gettftp") == 0)||(strcmp(strToken, "puttftp") == 0)){ // checking if command is gettftp or puttftp

						// getting the host and file
						strToken = strtok(NULL, separators);
						host = strToken;
						strToken = strtok(NULL, separators);
						file = strToken;

						//defining an addrinfo structure for UDP protocol
						struct addrinfo hints;
						memset(&hints,0,sizeof(struct addrinfo));
						hints.ai_family = AF_INET;
						hints.ai_protocol = IPPROTO_UDP;
						hints.ai_socktype = SOCK_DGRAM;

						struct addrinfo *res;
						int error;
						if ((error = getaddrinfo(host,PORT,&hints,&res)) != 0){
							if(write(STDOUT_FILENO,gai_strerror(error), strlen(gai_strerror(error))) == -1){perror("write");exit(EXIT_FAILURE);}}

						//creating a socket with the addrinfo structure 'res'
						int Server_socket;
						if((Server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){perror("Error creating socket");exit(EXIT_FAILURE);}

						//initilaizing the connection with the server
						if(connect(Server_socket, res->ai_addr, res->ai_addrlen) != 0){perror("connect");exit(EXIT_FAILURE);}

						freeaddrinfo(res);


					} else {strToken = strtok(NULL, separators);}
				}

				if(write(STDOUT_FILENO, nl, strlen(nl)) == -1){perror("write");exit(EXIT_FAILURE);} //setting new line

			}

		}
	exit(EXIT_SUCCESS);
}


