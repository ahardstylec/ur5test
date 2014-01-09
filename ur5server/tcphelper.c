#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>

char * get_interface_addr(char *interface, struct in_addr *ip_addr){
	// get ip addr of interface

	struct ifreq ifr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    //Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;
 
    //Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name , interface , IFNAMSIZ-1);
 
    if(ioctl(fd, SIOCGIFADDR, &ifr) == -1){
    	error("interface not found or offline");
    	close(fd);
		exit(1);
	}    
	close(fd);
 
	struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
	*ip_addr= ipaddr->sin_addr;
	return inet_ntoa(*ip_addr);
}