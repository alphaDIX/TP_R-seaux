/*
 * EX4.c
 *
 *  Created on: 19 déc. 2023
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

#define buffsize 516
#define datsize 512
#define PORT "69"
const char * zero = "00";
const char * one = "01";
const char * three = "03";
const char * four = "04";
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
						if((Server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
							perror("socket");freeaddrinfo(res);exit(EXIT_FAILURE);}

						//initializing the connection with the server
						if(connect(Server_socket, res->ai_addr, res->ai_addrlen) != 0){perror("connect");close(Server_socket);freeaddrinfo(res);exit(EXIT_FAILURE);}

						//LOOP OF EXCHANGES WITH SERVER
						//sending a RRQ request: opcode 01 + file_name + 00 + mode 'octet' + 00 (hexa)
						char * mode = "octet";
						char * RRQ = strcpy(RRQ,one);
						strcat(RRQ,file);
						strcat(RRQ,zero);
						strcat(RRQ,mode);
						strcat(RRQ,zero);

						if(send(Server_socket,RRQ,sizeof(RRQ),atoi(zero)) == -1){perror("send");close(Server_socket);freeaddrinfo(res);exit(EXIT_FAILURE);}

						//Receiving a data packet DAT: opcode 03 + block 01 + with acknowledgment ACK
						char DAT[datsize];
						int i = 0;
						for(i=0;i<buffsize;i++){buffer[i] = '\0';} //emptying buffer

						if(recv(Server_socket,buffer,buffsize,MSG_WAITALL) == -1){perror("recv");close(Server_socket);freeaddrinfo(res);exit(EXIT_FAILURE);}

						i = 4;
						while(i < strlen(buffer)){DAT[i] = buffer[i];}

						char * ACK = strcat(ACK,four);
						ACK = strcat(ACK,one);
						if(send(Server_socket,ACK,sizeof(ACK),atoi(zero)) == -1){perror("send");close(Server_socket);freeaddrinfo(res);exit(EXIT_FAILURE);}

						//closing socket and freeing address
						close(Server_socket);
						freeaddrinfo(res);


					} else {strToken = strtok(NULL, separators);}
				}

				if(write(STDOUT_FILENO, nl, strlen(nl)) == -1){perror("write");exit(EXIT_FAILURE);} //setting new line

			}

		}
	exit(EXIT_SUCCESS);
}

