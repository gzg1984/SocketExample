#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
int main(int argc, char *argv[])
{
	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;
	struct hostent *host;
	int portnumber,nbytes;
	if(argc!=3)
	{
		fprintf(stderr,"Usage:%s hostname portnumber\a\n",argv[0]);
		exit(1);
	}
	if((host=gethostbyname(argv[1]))==NULL)
	{
		fprintf(stderr,"Gethostname error\n");
		exit(1);
	}
	if((portnumber=atoi(argv[2]))<0)
	{
		fprintf(stderr,"Usage:%s hostname portnumber\a\n",argv[0]);
		exit(1);
	}
	/* 客户程序开始建立 sockfd描述符 */
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
		exit(1);
	}
	/* 客户程序填充服务端的资料 */
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(portnumber);
	server_addr.sin_addr=*((struct in_addr *)host->h_addr);
	/* 客户程序发起连接请求 
	 * 2.5 connect
	 * int connect(int sockfd, struct sockaddr * serv_addr,int addrlen)
	 * sockfd:socket返回的文件描述符.
	 * serv_addr:储存了服务器端的连接信息.其中sin_add是服务端的地址
	 * addrlen:serv_addr的长度
	 * connect函数是客户端用来同服务端连接的.成功时返回0,sockfd是同服务端通讯的文件
	 * 描述符 失败时返回-1.
	 * */
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr)
		  )==-1)
	{
		fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
		exit(1);
	}
	else
	{
		printf("Connect Success\n");
	}
	/* 连接成功了 */
	strcpy(buffer,"Test Send Static String");
	write(sockfd,buffer,strlen(buffer)+1);
	if((nbytes=read(sockfd,buffer,1024))==-1)
	{
		fprintf(stderr,"Read Error:%s\n",strerror(errno));
		exit(1);
	}
	buffer[nbytes]='\0';
	printf("I have received:%s\n",buffer);
	/* 结束通讯 */
	close(sockfd);
	exit(0);
}
