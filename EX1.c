/*
 * EX1.c
 *
 *  Created on: 11 déc. 2023
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

#define buffsize 128
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
						strToken = strtok(NULL, separators);
						host = strToken;
						strToken = strtok(NULL, separators);
						file = strToken;
					} else {strToken = strtok(NULL, separators);}

				}

				if(write(STDOUT_FILENO, nl, strlen(nl)) == -1){perror("write");exit(EXIT_FAILURE);} //setting new line

			}

		}
	exit(EXIT_SUCCESS);
}

