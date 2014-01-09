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
	int sockfd, newsockfd, portno;
	unsigned long * ip_addr;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	char * ip_addr_s= "";
	int iflag = 0;
	int port=8000;
	char * interface = "";
	int c;
   	opterr = 0;

	while ((c = getopt (argc, argv, "p:i:")) != -1)
        switch (c)
           {
            case 'p':
            	port = atoi(optarg);
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
    		interface ="lo";
    		ip_addr_s ="127.0.0.1";
    	}
    	if(inet_addr(ip_addr_s) == -1){
    		printf("ip address wrong");
    		exit(1);
    	}
    	*ip_addr= inet_addr(ip_addr_s);
    }else{
    	ip_addr_s = get_interface_addr(interface, ip_addr);
	}

 	printf ("port = %d, interface = %s, ip = %s\n", port, interface, ip_addr_s);
 	return 0;
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