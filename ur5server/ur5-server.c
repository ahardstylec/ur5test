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

int main(int argc, char **argv)
{
	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	int iflag = 0;
	int port= 8000;
	char * interface = "lo";
	char * ip_addr_s= "";
	char * garbage= NULL;
	int c;
   	opterr = 0;

	while ((c = getopt (argc, argv, "p:i:")) != -1)
        switch (c)
           {
            case 'p':
            	port = (int) strtol(argv[2], &garbage, 10);
            	if(port >65535){
        			exit(1);
    			}else if(port < 1024 && geteuid() != 0){
    				printf("ports below 1024 are priviliged! Run programm as root\n");
    				exit(1);
    			}
            	break;
            case 'i':
            	iflag=1;
             	interface = optarg;
            	break;
           	case '?':
             	if (optopt == 'p' || optopt == 'i')
               		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
             	else if (isprint (optopt))
               		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            	else
               		fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
             	return 1;
           	default:
           		abort ();
           }
     
    if(iflag == 0){
    	if(optind < argc){
			ip_addr_s = argv[optind];
    	}else{
    		interface = "lo";
    		ip_addr_s = "127.0.0.1";
    	}
	    	if(inet_aton(ip_addr_s, &serv_addr.sin_addr) == 0){
	    		printf("ip address not valid");
	    		exit(1);
	    	}
    }else{
    	ip_addr_s = get_interface_addr(interface, &serv_addr.sin_addr);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		error("ERROR opening socket");
	}
	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		printf("ERROR on binding on port %i", port);
		exit(1);
	}
 	printf("Server is running. listen on %s:%i\n", ip_addr_s, port);

	listen(sockfd,5);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		while(1) {
			bzero(buffer,256);
			n = read(newsockfd, buffer, 255);
			if (n < 0) error("ERROR reading from socket");
			printf("Here is the message: %s\n", buffer);
			n = write(newsockfd, buffer , sizeof(buffer);
			if (n < 0) error("ERROR writing to socket");
		}
		close(newsockfd);
		close(sockfd);
	//}
	return 0;
}