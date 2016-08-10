#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>  /** for strerror **/
#include <stdlib.h>  /** for exit **/
#include <unistd.h>  /** for close **/


#define SERVER_PORT 8889
#define MAX_MSG_SIZE 1024

void udps_respon(int sockfd)
{
	struct sockaddr_in addr;
	int n;
	socklen_t addrlen;
	char msg[MAX_MSG_SIZE];
	while(1)
	{ /* 从网络上读,写到网络上面去 */
		/*
		 * int recvfrom(int sockfd,void *buf,int len,unsigned int flags,struct socka
		 * ddr * from int *fromlen)
		 * int sendto(int sockfd,const void *msg,int len,unsigned int flags,struct s
		 * ockaddr *to int tolen)
		 * sockfd,buf,len的意义和read,write一样,分别表示套接字描述符,发送或接收的缓冲区
		 * 及大小.recvfrom负责从sockfd接收数据,如果from不是NULL,那么在from里面存储了信息
		 * 来源的情况,如果对信息的来源不感兴趣,可以将from和fromlen设置为NULL.sendto负责
		 * 向to发送信息.此时在to里面存储了收信息方的详细资料.
		 * */
		n=recvfrom(sockfd,msg,MAX_MSG_SIZE,0,
				(struct sockaddr*)&addr,&addrlen);
		msg[n]=0;
		/* 显示服务端已经收到了信息 */
		fprintf(stdout,"I have received %s from %s\n",msg,inet_ntoa(addr.sin_addr));
		sendto(sockfd,msg,n,0,(struct sockaddr*)&addr,addrlen);
	}
}
int main(int ac, char** av)
{
	int sockfd;
	struct sockaddr_in addr;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		fprintf(stderr,"Socket Error:%s\n",strerror(errno));
		exit(1);
	}
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(SERVER_PORT);
	if(bind(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))<0
	  )
	{
		fprintf(stderr,"Bind Error:%s\n",strerror(errno));
		exit(1);
	}
	udps_respon(sockfd);
	close(sockfd);
}
