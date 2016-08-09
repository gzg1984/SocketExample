#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sockfd,new_fd;
	struct sockaddr_in server_addr;
	//struct sockaddr_in client_addr;
	struct sockaddr_in client_addr;
	unsigned int sin_size;
	int portnumber;
	char hello[]="Hello! Are You Fine?\n";
	if(argc!=2)
	{
		fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);
		exit(1);
	}
	if((portnumber=atoi(argv[1]))<0)
	{
		fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);
		exit(1);
	}
	/* 服务器端开始建立socket描述符 */
	/*
	 * 2.1 socket
	 * int socket(int domain, int type,int protocol)
	 * domain:说明我们网络程序所在的主机采用的通讯协族(AF_UNIX和AF_INET等). AF_UN
	 * IX只能够用于单一的Unix系统进程间通信,而AF_INET是针对Internet的,因而可以允许在
	 * 远程 主机之间通信(当我们 man socket时发现 domain可选项是 PF_*而不是AF_*,因为
	 * glibc是posix的实现 所以用PF代替了AF,不过我们都可以使用的).
	 * type:我们网络程序所采用的通讯协议(SOCK_STREAM,SOCK_DGRAM等) SOCK_STREAM表明
	 * 我们用的是TCP协议,这样会提供按顺序的,可靠,双向,面向连接的比特流. SOCK_DGRAM 
	 * 表明我们用的是UDP协议,这样只会提供定长的,不可靠,无连接的通信.
	 * protocol:由于我们指定了type,所以这个地方我们一般只要用0来代替就可以了 sock
	 * et为网络通讯做基本的准备.成功时返回文件描述符,失败时返回-1,看errno可知道出错
	 * 的详细情况.
	 * */
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
		exit(1);
	}
	/* 服务器端填充 sockaddr结构 */
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(portnumber);
	/* 捆绑sockfd描述符 */
	/*2.2 bind
	 * int bind(int sockfd, struct sockaddr *my_addr, int addrlen)
	 * sockfd:是由socket调用返回的文件描述符.
	 * addrlen:是sockaddr结构的长度.
	 * my_addr:是一个指向sockaddr的指针. 在<linux/socket.h>;中有 sockaddr的定义
	 * struct sockaddr{
	 * unisgned short as_family;
	 * char sa_data[14];
	 * };
	 * 不过由于系统的兼容性,我们一般不用这个头文件,而使用另外一个结构(struct sock
	 * addr_in) 来代替.在<linux/in.h>;中有sockaddr_in的定义
	 * struct sockaddr_in{
	 * unsigned short sin_family;
	 * unsigned short int sin_port;
	 * struct in_addr sin_addr;
	 * unsigned char sin_zero[8];
	 * 我们主要使用Internet所以sin_family一般为AF_INET,sin_addr设置为INADDR_ANY表
	 * 示可以 和任何的主机通信,sin_port是我们要监听的端口号.sin_zero[8]是用来填充的
	 * .. bind将本地的端口同socket返回的文件描述符捆绑在一起.成功是返回0,失败的情况和
	 * socket一样*/
	if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==
			-1)
	{
		fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
		exit(1);
	}
	/* 监听sockfd描述符 
	 * int listen(int sockfd,int backlog)
	 * sockfd:是bind后的文件描述符.
	 * backlog:设置请求排队的最大长度.当有多个客户端程序和服务端相连时, 使用这个表示
	 * 可以介绍的排队长度. listen函数将bind的文件描述符变为监听套接字.返回的情况和b
	 * ind一样.*/
	if(listen(sockfd,5)==-1)
	{
		fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
		exit(1);
	}
	while(1)
	{
		/* 服务器阻塞,直到客户程序建立连接 */
		sin_size=sizeof(struct sockaddr_in);
		/*
		 * 2.4 accept
		 * int accept(int sockfd, struct sockaddr *addr,int *addrlen)
		 * sockfd:是listen后的文件描述符.
		 * addr,addrlen是用来给客户端的程序填写的,服务器端只要传递指针就可以了. bind,li
		 * sten和accept是服务器端用的函数,accept调用时,服务器端的程序会一直阻塞到有一个
		 * 客户程序发出了连接. accept成功时返回最后的服务器端的文件描述符,这个时候服务
		 * 器端可以向该描述符写信息了. 失败时返回-1
		 * */
		if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size
				 ))==-1)
		{
			fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
			exit(1);
		}
		printf("char * inet_ntoa(struct in_addr in); /** Need <arpa/inet.h> or this APP will segment Fault**/\n");
		printf("    Connected From client.in_addr %s\n", inet_ntoa(client_addr.sin_addr));
		if(write(new_fd,hello,strlen(hello))==-1)
		{
			fprintf(stderr,"Write Error:%s\n",strerror(errno));
			exit(1);
		}
		/* 这个通讯已经结束 */
		close(new_fd);
		/* 循环下一个 */
	}
	close(sockfd);
	exit(0);
}

