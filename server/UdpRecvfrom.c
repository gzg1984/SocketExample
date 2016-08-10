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

void udp_recvfrom(int sockfd)
{
	struct sockaddr_in addr;
	int n;
	socklen_t addrlen;
	char msg[MAX_MSG_SIZE];
	while(1)
	{ /* 从网络上读,写到网络上面去 */
		n=recvfrom(sockfd,msg,MAX_MSG_SIZE,0,
				(struct sockaddr*)&addr,&addrlen);
		msg[n]=0;
		/* 显示服务端已经收到了信息 */
		fprintf(stdout,"I have received %s",msg);
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
	if(bind(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))<0)
	{
		fprintf(stderr,"Bind Error:%s\n",strerror(errno));
		exit(1);
	}
	udp_recvfrom(sockfd);
	close(sockfd);
}
