#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <sched.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../libs/ur5.h"

using namespace std;

int main(int argc, char **argv){
	int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    struct ur5_data * ur5_d;
    ur5_d->size = sizeof(*ur5_d);
    bzero(ur5_d, ur5_d->size);
    char * ip_string="";
    char *garbage=NULL;
    char buf[ur5_d->size];
	bzero(&serveraddr, sizeof(serveraddr));

	if (argc != 3) {
       fprintf(stderr,"usage: %s <ip> <port>\n", argv[0]);
       exit(1);
    }

    ip_string = argv[1];
    if(inet_aton(ip_string, &serveraddr.sin_addr) == 0){
            printf("ip address not valid");
            exit(1);
    }

    portno = (int) strtol(argv[2], &garbage, 10);
    printf("Start UR5 Client\n");
    printf("connecting to server %s:%d..", ip_string, portno);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        printf("failed\n");
        error("ERROR opening socket");
        exit(1);
	}
    /* build the server's Internet address */
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(portno);

    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0){
        printf("failed\n");
        error("ERROR connecting");  
    }
    printf("suceess\n");
    /* get message line from the user */
    while(1){

        /* print the server's reply */
        bzero(buf, ur5_d->size);
        read(sockfd, buf, sizeof(*ur5_d));
        ur5_d= (struct ur5_data *) buf;

        if (n < 0)
          error("ERROR reading from socket");
        
        /* send the message line to the server */
        n = write(sockfd, (char *) ur5_d, sizeof(*ur5_d));
        if (n < 0) 
          error("ERROR writing to socket");
        
        printf("Echo from server: %s", buf);    
    }
    
    close(sockfd);
    return 0;
}