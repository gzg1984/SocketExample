#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>   //ip
#include <netinet/tcp.h> //tcp
#include <stdlib.h>
char srcip[]="100.100.100.100",dstip[]="192.168.11.220";
int srcport=25,dstport=99;
/*
struct ip
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ip_hl:4;                //little-endian IPͷ����(��λΪ32λ)4λ
    unsigned int ip_v:4;                 //�汾��4 IP4��4 
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
    unsigned int ip_v:4;                // version 
    unsigned int ip_hl:4;               // header length 
#endif
    u_int8_t ip_tos;                    //��������  һ��Ϊ0
    u_short ip_len;                     //�����ܳ��� (��λΪ32λ)
    u_short ip_id;                      //��ʶ16 
    u_short ip_off;                     //�ֶ�ƫ�� 
#define IP_RF 0x8000                    // reserved fragment flag  //��־
#define IP_DF 0x4000                    // dont fragment flag 
#define IP_MF 0x2000                    // more fragments flag 
#define IP_OFFMASK 0x1fff               // mask for fragmenting bits 
    u_int8_t ip_ttl;                     //����ʱ�� 
    u_int8_t ip_p;                      //����Э�� tcp��6 
    u_short ip_sum;                     //ͷУ���
    struct in_addr ip_src, ip_dst;      // Դ��ַ Ŀ���ַ 
  };

struct tcphdr
  {
    u_int16_t source;               // Դ�˿� 
    u_int16_t dest;                // Ŀ�Ķ˿� 
    tcp_seq seq;                  // ��� 
    tcp_seq ack_seq;             // ȷ�Ϻ� �յ���TCP��Ϣ�����+1 
#if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t res1:4;               //����
    u_int16_t doff:4;               //����
    u_int16_t fin:1;               //��־ ����
    u_int16_t syn:1;               //��־ ͬ��
    u_int16_t rst:1;               //��־ ����
    u_int16_t psh:1;               //��־ ��ջ
    u_int16_t ack:1;               //��־ ȷ��
    u_int16_t urg:1;               //��־ ���� 
    u_int16_t res2:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
    u_int16_t doff:4;
    u_int16_t res1:4;
    u_int16_t res2:2;
    u_int16_t urg:1;
    u_int16_t ack:1;
    u_int16_t psh:1;
    u_int16_t rst:1;
    u_int16_t syn:1;
    u_int16_t fin:1;
#else
#error  "Adjust your <bits/endian.h> defines"
#endif
    u_int16_t window;           //����
    u_int16_t check;            //У���
    u_int16_t urg_ptr;          //����ָ��
};
*/

//У��ͺ��� ipͷ
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
//У��ͺ��� tcpͷ
unsigned short tcpcsum (unsigned char *iphdr, unsigned short *packet,int packlen) {
	unsigned short *buf;
	unsigned short res;
	buf = malloc(packlen+12);
	if(buf == NULL) return 0;
	memcpy(buf,iphdr+12,8); //ԴIP��ַ��Ŀ��IP��ַ
	*(buf+4)=htons((unsigned short)(*(iphdr+9)));
	*(buf+5)=htons((unsigned short)packlen);
	memcpy(buf+6,packet,packlen);
	res = csum(buf,packlen+12);
	free(buf);
	return res;
}

int main()
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
	//���͵�ַ
	toaddr.sin_family =AF_INET;
	//        inet_aton("192.168.11.38",&toaddr.sin_addr);   //�ַ���ת���ַ
	inet_aton(dstip,&toaddr.sin_addr);   //�ַ���ת���ַ
	//����ԭʼTCP����ʽIP+TCP��Ϣ��
	sock = socket(AF_INET, SOCK_RAW,IPPROTO_RAW);   //IP��ʽ
	//	    sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (sock>0) {printf("socket ok\n");}
	else {printf ("socket error \n");}
	n=1;
	if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &n, sizeof(n)) < 0) { 
		printf("2");
		perror("IP_HDRINCL"); 
		exit(1); 
	}
	ip=(struct ip *)buffer;
	ip->ip_id=0x9911;                
	ip->ip_v=4;
	ip->ip_hl=5;                            
	ip->ip_ttl=36;                          
	ip->ip_p=6;     //tcp Ϊ6
	ip->ip_len=htons(60);
	inet_aton(dstip,&ip->ip_dst);
	//        inet_aton("192.168.11.38",&ip->ip_dst);
	tcp = (struct tcphdr *)(buffer + (4*ip->ip_hl));
	tcp->source=htons(srcport);                            //Դ�˿�
	tcp->dest=htons(dstport);   
	tcp->seq=htons(0x9990);
	tcp->doff=0x15;
	tcp->ack_seq=0;
	tcp->syn=1;
	tcp->window=htons(0x20);
	inet_aton(srcip,&ip->ip_src);

	//       	ip->ip_src.s_addr=htonl(0x09010101+n);
	tcp->check=0x0;        
	tcp->check=tcpcsum((unsigned char *)buffer, (unsigned short *)tcp,40);
	ip->ip_sum=0;
	ip->ip_sum=csum((unsigned short *)buffer,20);
	bytes_send=sendto(sock,buffer,60,0,(struct sockaddr *)&toaddr,sizeof(toaddr));     

	if (bytes_send>0)
	{
		printf("OK bytes_send %d \n",bytes_send);
		printf("IP_source address ::: %s \n",inet_ntoa(ip->ip_src));
		printf("IP_dest address ::: %s \n",inet_ntoa(ip->ip_dst));
	}
}

