/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcphelper.h"
#include <ur5.h>

using namespace std;

int main(int argc, char **argv)
{
	struct sockaddr_in serv_addr, cli_addr;
   	
   	struct ur5_data * ur5_d= create_ur5_data();
   	struct ur5_data * ur5_new_data= create_ur5_data();
	struct ur5_data * ur5_d_temp;

    ur5_d->tcp[TCP_X] = 40;
    ur5_d->tcp[TCP_Y] = 30;
    ur5_d->tcp[TCP_Z] = 20;
	
	int sockfd, newsockfd, c, n;
	int iflag = 0;
	int port= 8000;

	char * interface = "lo";
	char * ip_addr_s;
	char * garbage= NULL;
   	char buffer[ur5_d->size];

	float average_time= 0.0;
   	opterr = 0;
	socklen_t clilen;

	while ((c = getopt (argc, argv, "p:i:")) != -1)
        switch (c)
           {
            case 'p':
            	port = (int) strtol(optarg, &garbage, 10);
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
		newsockfd = accept(sockfd, (struct sockaddr  *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		printf("client connected: %s\n", inet_ntoa(cli_addr.sin_addr));
		unsigned long int iterations=0;
		unsigned long int packages_send=0;
		unsigned long int packages_recieved=0;
		unsigned long int errors=0;
		unsigned long int average=0;
		struct timeval stop, start;
		
		while(iterations<1000) {
			gettimeofday(&start, NULL);
//---------------------------- send  -------------------------------------------------------------------
			n = write(newsockfd, (char*) ur5_d , sizeof(*ur5_d));
			if (n < 0){
				errors++;
				continue;
				//error("ERROR writing to socket");
			}
			packages_send++;
//------------------------------------------------------------------------------------------------------

//---------------------------- wait for client or timeout ----------------------------------------------
			
//---------------------------- recieve -----------------------------------------------------------------
			read(newsockfd, ur5_new_data, sizeof(*ur5_d));

			if (n < 0){
				errors++;
				continue;
				//error("ERROR reading from socket");	
			}
			packages_recieved++;
//------------------------------------------------------------------------------------------------------

//---------------------------- do stuff with data ------------------------------------------------------

		// ....
			ur5_d_temp = ur5_d;
			ur5_d = ur5_new_data;

//------------------------------------------------------------------------------------------------------
			gettimeofday(&stop, NULL);
			long int time_in_microsec = 8000 - (stop.tv_usec - start.tv_usec);
			if(time_in_microsec > 0 && time_in_microsec < 8000){
				usleep(time_in_microsec);
			}else if(time_in_microsec > 8000){
				errors++;
			}
			gettimeofday(&stop, NULL);
			average_time = average_time + (stop.tv_usec - start.tv_usec);
			iterations++;
			printf("\rpackages send: %d", iterations);
			fflush(stdout);
		}

		close(newsockfd);
		close(sockfd);

		puts("connection to client closed");
		printf("average connection time: %10.4fmsec\n", average_time/iterations / 1000);
		printf("packages send: %d\n", packages_send);
		printf("packages recieved: %d\n", packages_recieved);
		printf("errors occured: %d\n", errors);
		puts("\n");
		print_ur5_data(ur5_d);
	//}
	return 0;
}