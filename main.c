#include <linux/if_tun.h>
#include<pthread.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/if.h>
#include <error.h>
#include <string.h>   //memset
#include <arpa/inet.h> //ntohs
#include "tap.h"
int readt(struct args *args){
	int fd=0;
	//fd=args->sock;
	char buf[255];
	int size =0;
	while(1){
		memset(buf,0,255);
		size=read(args->sock,buf,255);
		if(size<0)exit(1);
		printf("%d,%s\n",size,buf);
		size=write(args->sock,buf,size);
		if(size<0)exit(1);
	}
}

pthread_mutex_t mut;
pthread_t  *thread=0;

int udata(char *buf,int size ){
	for(int i=0;i<size;i++)
		buf[i]--;
}

int edata(char *buf,int size){
	for(int i=0;i<size;i++)
		buf[i]++;

}

int  t2p(struct args *args){
	printf("t2p dddd: %d\n",args->sock);
	//	 return 1;
	int size =0;
	char *buf=malloc(1024);
	if(buf==0)
		exit(1);
	while(1){
		/*	if(args->sock==-1){
		//sleep(1);
		fprintf(stderr,"11111\n");
		usleep(50000);
		continue ;
		}*/
		size =read(args->tap,buf,1024);
		if(size<0)
			fprintf(stderr,"err: read tap\n");
		edata(buf,size);
		fprintf(stderr,"tap: read  %d\n",size);
		if( args->sock!=-1){
			size=write(args->sock,buf,size);
			if(size<0){
				close(args->sock);
				args->sock=-1;
				fprintf(stderr,"err: write  tcp\n");
				//continue ;
			}
			if(size>0)fprintf(stderr,"tcp: write  %d\n",size);
		}
		//if(size>0)fprintf(stderr,"tcp: write  %d\n",size);

	}

}
int p2t(struct args *args){
	//readt(args);
	printf("p2t dddd: %d\n",args->sock);
	// return 1;
	int size =0;
	char *buf=malloc(1024);
	if(buf==0)
		exit(1);	
	while(1){
		// fprintf(stderr,"11111\n");
		/*if(args->sock==-1)
		  {
		  fprintf(stderr,"11111\n");
		  sleep(1);
		  usleep(50000);
		  continue ;
		  }*/
		if(args->sock==-1){
	//		printf("ddddddddd ");
			usleep(50000);
			continue ;
		}
		
			size=read(args->sock,buf,1024);
			if(size<1){
				fprintf(stderr,"err: read  tcp\n");
				close(args->sock);
				args->sock=-1;
				continue ;
			}
			if(size>0)fprintf(stderr,"tcp: read  %d\n",size);
		
		//	if(size>0)fprintf(stderr,"tcp: read  %d\n",size);
		udata(buf,size);   
		size=write(args->tap,buf,size);
		if( size<1)
			fprintf(stderr,"err: write tap\n");
		else
			fprintf(stderr,"tap: write  %d\n",size);

	}
}


//int tap_create(char *dev, int flags);







void thread_create(struct args *args){
	int temp;
	//readt(args->tap);
	//exit(1);
	//	 printf("pth dddd: %d\n",args->sock);
	thread=(pthread_t *)malloc(sizeof(pthread_t)*2);
	memset(thread,0,sizeof(pthread_t)*2);

	if((temp = pthread_create(&thread[0], NULL, t2p, args)) != 0)
		printf("线程%d创建失败!\n",0);
	if((temp = pthread_create(&thread[1], NULL, p2t, args)) != 0)
		printf("线程%d创建失败!\n",1);
	//	while(1)sleep(1);



}






int print_help(){
	fprintf(stderr, "  -a  --add {ip}  连接目标地址\n");
	fprintf(stderr, "  -t  --tap {name}  tap name/tap1\n");
	//	fprintf(stderr, "  -s  --sport {port}  连接目标端口\n");
	fprintf(stderr, "  -p   --port {port}  端口\n");
}







void cli_parse(struct args *args , int argc, char *argv[]) {
	int opt;
	struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'V'},
		{"port", required_argument, NULL, 'p'},
		//		{"sport", required_argument, NULL, 's'},
		{"addr", required_argument, NULL, 'a'},
		{"tap", required_argument, NULL, 't'},
		{NULL, 0, NULL, 0},
	};
	while ((opt = getopt_long(argc, argv, "vha:p:t:", long_options, NULL)) != -1) {
		switch (opt) {
			case 'h':
				print_help();
				exit(0);
			case 'v':
				print_help();
				exit(0);
			case 'p':
				args->port=atoi(optarg);
				break;/*
					 case 's': 
					 args->sport=atoi(optarg);
					 break;*/
			case 'a':
				args->add=optarg;
				args->con=1;
				break;
			case 't':
				args->dev=optarg;
				break;
			default:
				exit(1);
		}
	}
	if (argv[optind]) {
		fprintf(stderr, "error: extra argument given: %s\n", argv[optind]);
		exit(1);
	}
}





int main(int argc, char *argv[])
{
	struct args rgs;
	struct args *args;
	args=&rgs;
	memset(args, 0, sizeof args);
	args->port = 10087;
	args->dev="tap01";
	args->tap=-1;
	args->con=0;
	args->sock=-1;
	args->add="0.0.0.0";
	cli_parse(args, argc, argv);


	int ret,i,ret_all=0,time=0,pkts=0;
	//unsigned char buf[1024];
	printf("%s\n",args->dev);
	//exit (1);
	args->tap = tap_create(args);
	//tap1 = tap_create("tap1", IFF_TAP | IFF_NO_PI);
	//exit (1);
	if (args->tap < 0) {
		perror("错误： tap_create");
		return 1;
	}
	//exit (1);
	printf("TAP name is %s\n", args->dev);
	//exit (1);
	//fcntl(args->tap, F_SETFL, O_NONBLOCK);
	//	char bb[1024];
	//	int uu=args->tap;
	//	int m=
	//	read(uu,bb,1024);
	//printf("mmmm: %d,%d\n",args->tap,m);
	//exit(1);
	//readt(args->tap);
	//exit(0);
	wtcps(args);
	thread_create(args);
	printf("主dddd: %d\n",args->sock);
	while (1) 
	{
		//	memset(buf,0,1024);

		if(args->sock==-1){
			printf("主dddd: %d\n",args->sock);
			comm(args);
			printf("主dddd: %d\n",args->sock);
			usleep(50000);
			//			continue ;	 
		}
		//		size=read(args->sock,buf,1024);
		//		if(size<1)
		//			break;
		//		printf("%d,%s\n'",size,buf);


	}


	return 0;
}









/*





   int tap_create(char *dev, int flags)
   {
   struct ifreq ifr;
   int fd, err;


   if ((fd = open("/dev/tun", O_RDWR|O_NONBLOCK)) < 0)
   return fd;


   memset(&ifr, 0, sizeof(ifr));
   ifr.ifr_flags |= flags;
   if (*dev != '\0')
   strncpy(ifr.ifr_name, dev, IFNAMSIZ);
   if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
   close(fd);
   return err;
   }
//exit(0);
//strcpy(dev, ifr.ifr_name);


return fd;
}

tderr,"err: read  tcp\n");/用例：tap_setMAC("tap100","08:00:11:22:33:44")
signed char tap_setMAC(const unsigned char *interface_name, const unsigned char *str_macaddr)
{
int             ret;
int             sock_fd;
struct ifreq    ifr;
unsigned int    mac2bit[6];

if(interface_name == NULL || str_macaddr == NULL)
{
return -1;
}


//提取mac格式
sscanf((char *)str_macaddr, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned int *)&mac2bit[0], (unsigned int *)&mac2bit[1], (unsigned int *)&mac2bit[2], (unsigned int *)&mac2bit[3], (unsigned int *)&mac2bit[4], (unsigned int *)&mac2bit[5]);

sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
if (sock_fd < 0)
{
return -2;
}

sprintf(ifr.ifr_ifrn.ifrn_name, "%s", interface_name);
ifr.ifr_ifru.ifru_hwaddr.sa_family = 1;
ifr.ifr_ifru.ifru_hwaddr.sa_data[0] = mac2bit[0];
ifr.ifr_ifru.ifru_hwaddr.sa_data[1] = mac2bit[1];
ifr.ifr_ifru.ifru_hwaddr.sa_data[2] = mac2bit[2];
ifr.ifr_ifru.ifru_hwaddr.sa_data[3] = mac2bit[3];
ifr.ifr_ifru.ifru_hwaddr.sa_data[4] = mac2bit[4];
ifr.ifr_ifru.ifru_hwaddr.sa_data[5] = mac2bit[5];

ret = ioctl(sock_fd, SIOCSIFHWADDR, &ifr);
if (ret != 0)
{
return -4;
}

close( sock_fd );


return 0;
}
*/

