#include <fcntl.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <sys/ioctl.h>  
#include <linux/if.h>  
#include <linux/if_tun.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <net/route.h>  
#include "tap.h"
#include <unistd.h>
/** 
 *  创建接口 
 */ 
int tap_clone(struct args *args){
if(args->tap);
close(args->tap);
}
int tap_create(struct args *args)  
{ 
       if(args->flags==0)
	       args->flags=IFF_TAP | IFF_NO_PI;	
	struct ifreq ifr;  
	int  err;  
	if ((args->tap = open("/dev/tun", O_RDWR)) < 0)  
	{  
		printf("Error :%m/n", errno);  
		return -1;  
	} 
	memset(&ifr, 0, sizeof(ifr));  
	ifr.ifr_flags |=  args->flags;  
  
	if (args->dev != '/0')  
	{  
		strncpy(ifr.ifr_name, args->dev, IFNAMSIZ);  
	}  
	if ((err = ioctl(args->tap, TUNSETIFF, (void *)&ifr)) < 0)   
	{  
		printf("Error :%m/n", errno);  
		close(args->tap);  
		return -1;  
	}  
	//strcpy(dev, ifr.ifr_name);  
	return args->tap;  
}/*  
iimain(){

int tap_fd = tun_create("testtap",IFF_TAP|IFF_NO_PI);
int u=0;
char buf[1024];
while (1){
u=read(tap_fd,buf,1024);
printf( "%d,%d\n",tap_fd,u);
}
close(tap_fd);
}*/
