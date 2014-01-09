/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcphelper.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	unsigned long * ip_addr;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	switch(argc){
		case 1:
			*ip_addr = inet_addr("127.0.0.1");
			if(*ip_addr == -1){
				exit(0);
			}
			portno=8000;
			break;
		case 2:
			get_interface_addr(argv[1], ip_addr);
			portno = 8000;
			break;
		case 3:
			get_interface_addr(argv[1], ip_addr);
			portno = atoi(argv[2]);
			break;
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		error("ERROR opening socket");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr= *ip_addr;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		printf("ERROR on binding on port %i", portno);
		exit(1);
	}
	char * ip_addr_s = inet_ntoa(((struct sockaddr_in *) &serv_addr)->sin_addr);
 	printf("Server is running. listen on %s:%i\n", ip_addr_s, portno);
	return 0;
	listen(sockfd,5);
	//while(1) {
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");

		bzero(buffer,256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) error("ERROR reading from socket");
		printf("Here is the message: %s\n", buffer);
		n = write(newsockfd,"I got your message",18);
		if (n < 0) error("ERROR writing to socket");
		close(newsockfd);
		close(sockfd);
	//}
	return 0; 
}