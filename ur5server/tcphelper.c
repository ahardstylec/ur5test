#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>

char * get_interface_addr(char *interface, unsigned long *ip_addr){
	// get ip addr of interface eth0
	

	struct ifreq ifr;
	size_t if_name_len=strlen(interface);
	if (if_name_len<sizeof(ifr.ifr_name)) {
	    memcpy(ifr.ifr_name,interface,if_name_len);
	    ifr.ifr_name[if_name_len]=0;
	} else {
	    error("interface name is too long");
	}

	int fd=socket(AF_INET,SOCK_DGRAM,0);
	if (fd==-1) {
	    error(strerror(errno));
	}

	if (ioctl(fd, SIOCGIFADDR, &ifr)==-1) {
	    error("interface not found");
	    close(fd);
	    exit(0);
	}

	struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
	*ip_addr= ipaddr->sin_addr.s_addr;
	printf("IP address: %s\n", inet_ntoa(((struct sockaddr_in *)&ipaddr)->sin_addr));
	return 0;
}