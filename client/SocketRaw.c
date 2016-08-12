#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>   //ip
#include <netinet/tcp.h> //tcp
#include <stdlib.h>
#include <string.h> /** memcpy **/

char srcip[]="100.100.100.100",dstip[]="192.168.11.220";
int srcport=25,dstport=99;
unsigned short csum (unsigned short *packet, int packlen) {
	register unsigned long sum = 0;
	while (packlen > 1) {
		sum+= *(packet++);
		packlen-=2;
	}
	if (packlen > 0)
		sum += *(unsigned char *)packet;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

	return (unsigned short) ~sum;
}
unsigned short tcpcsum (unsigned char *iphdr, unsigned short *packet,int packlen) 
{
	unsigned short *buf;
	unsigned short res;
	buf = malloc(packlen+12);
	if(buf == NULL) return 0;
	memcpy(buf,iphdr+12,8);
	*(buf+4)=htons((unsigned short)(*(iphdr+9)));
	*(buf+5)=htons((unsigned short)packlen);
	memcpy(buf+6,packet,packlen);
	res = csum(buf,packlen+12);
	free(buf);
	return res;
}

int main(int ac, char** av)
{

	int sock, bytes_send, fromlen,n,id,s;
	unsigned char buffer[65535];
	struct sockaddr_in toaddr;
	struct ip  *ip;
	struct tcphdr *tcp;
	for (n=0;n<60000;n++ )
	{
		buffer[n]=0;
	}
	toaddr.sin_family =AF_INET;
	inet_aton(dstip,&toaddr.sin_addr);   
	sock = socket(AF_INET, SOCK_RAW,IPPROTO_RAW); 
	if (sock>0) 
	{
		printf("socket ok\n");
	}
	else 
	{
		printf ("socket error \n");
	}
	n=1;
	if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &n, sizeof(n)) < 0) 
	{
		printf("2");
		perror("IP_HDRINCL"); 
		exit(1); 
	}
	ip=(struct ip *)buffer;
	ip->ip_id=0x9911;                
	ip->ip_v=4;
	ip->ip_hl=5;                            
	ip->ip_ttl=36;                          
	ip->ip_p=6;     
	ip->ip_len=htons(60);
	inet_aton(dstip,&ip->ip_dst);
	tcp = (struct tcphdr *)(buffer + (4*ip->ip_hl));
#ifdef __APPLE__
	tcp->th_sport =htons(srcport);                     
	tcp->th_dport =htons(dstport);   
	tcp->th_seq =htons(0x9990);
	tcp->th_ack = 0x0;
	tcp->th_x2 = 0x0;
	tcp->th_off = sizeof(struct tcphdr) + 1;
	tcp->th_flags = TH_SYN;
	tcp->th_win = htonl(0x20);
	tcp->th_sum =tcpcsum((unsigned char *)buffer, (unsigned short *)tcp,40);
	tcp->th_urp = 0x0;
#else
#ifdef __BSD__
	tcp->th_source=htons(srcport);                       
	tcp->th_dest=htons(dstport);   
	tcp->th_seq=htons(0x9990);
	tcp->th_doff=0x15;
	tcp->th_ack_seq=0;
	tcp->th_syn=1;
	tcp->th_window=htons(0x20);
	tcp->th_check=tcpcsum((unsigned char *)buffer, (unsigned short *)tcp,40);
#else
	tcp->source=htons(srcport);                       
	tcp->dest=htons(dstport);   
	tcp->seq=htons(0x9990);
	tcp->doff=0x15;
	tcp->ack_seq=0;
	tcp->syn=1;
	tcp->window=htons(0x20);
	tcp->check=0x0;        
	tcp->check=tcpcsum((unsigned char *)buffer, (unsigned short *)tcp,40);
#endif
#endif
	inet_aton(srcip,&ip->ip_src);
	ip->ip_sum=0;
	ip->ip_sum=csum((unsigned short *)buffer,20);
	bytes_send=sendto(sock,buffer,  /** buffer contain IP **/
			60,0,
			(struct sockaddr *)&toaddr,  /** No destiny IP**/
			sizeof(toaddr));     

	if (bytes_send>0)
	{
		printf("OK bytes_send %d \n",bytes_send);
		printf("IP_source address ::: %s \n",inet_ntoa(ip->ip_src));
		printf("IP_dest address ::: %s \n",inet_ntoa(ip->ip_dst));
	}
}
