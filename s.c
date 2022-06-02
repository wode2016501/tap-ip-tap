#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "tap.h"
//#define IP "0.0.0.0"
//#define PORT 6666


int comm(struct args *args){
	int len=0,ret=0;
	if(args->con){
		//	printf("客户端！\n",);
		len=sizeof(args->addr);
		printf("客户端！%d\n",len);
		ret=connect(args->list_fb,(struct sockaddr *)&(args->addr),len);
		if(ret!=-1){
			args->sock=args->list_fb;

		printf("连接服务器成功！%d\n",args->list_fb);
		}
		 return ret;
	}
	else
	{
		//printf("服务器！\n");
		len=sizeof(args->caddr);
		printf("服务器！%d\n",len);
		args->sock = accept( args->list_fb,(struct sockaddr *)&(args->caddr),&len);
		printf("与%s链接成功！\n",inet_ntoa(args->caddr.sin_addr));
	}
	return  args->sock;
}

int cltcp(struct args *args){
	if(args->sock)close(args->sock);
	if(args->list_fb)close(args->list_fb);
}


int wtcps(struct args *args)
{
	int ret=0;
	//	char buf[512]={0};
	//	struct sockaddr_in list_buf,Data_buf;
	//	建立监听套接字
	args->list_fb=socket(AF_INET,SOCK_STREAM,0);
	if(args->list_fb<0)
	{
		perror("err: socket");
		exit(1);
	}
	//	绑定IP地址和端口号
	args->addr.sin_family=AF_INET;
	args->addr.sin_port=htons(args->port);
	args->addr.sin_addr.s_addr=inet_addr(args->add);
	if(args->con)
		return args->list_fb;
	//printf("size: %d\n",sizeof(args->addr));
	ret=bind( args->list_fb,(struct sockaddr *)&args->addr,sizeof(args->addr));
	if(ret == -1)
	{
		perror("bind");
		return -1;
	}
	printf("绑定成功！\n");
	//	开始监听
	ret=listen(args->list_fb,2);
	if(ret == -1)
	{
		perror("listen");
		return -1;
	}
	printf("监听成功！\n");

	//	等待链接
	//len=sizeof(args->caddr);
	//	args->sock = accept( args->list_fb,(struct sockaddr *)&(args->caddr),&len);
	//	printf("与%s链接成功！\n",inet_ntoa(args->caddr.sin_addr));
	return args->list_fb;
}
