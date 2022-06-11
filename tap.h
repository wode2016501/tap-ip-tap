#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/in.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <errno.h>
#include <net/route.h>
#include <unistd.h>
//int tcps(struct args args);
struct args {
	char *add;
	char *dev;
	int port;
	int con;
	int tap;
	int sock;
	 int list_fb;
struct sockaddr_in addr;
struct sockaddr_in caddr;
int flags;
};
//struct sockaddr_in addr,caddr = {0};
int wtcps(struct args *args);
int comm( struct args *args);
