#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // Include this for inet_addr
#include <unistd.h>     // Include this for close

// Used lecture slides and textbook to complete code.


// icmp header
struct icmpheader
{
    
    unsigned char       icmp_type;
    unsigned char       icmp_code;
    unsigned short int  icmp_chksum;
  unsigned short int  icmp_id; // identify request 
  unsigned short int  icmp_seq; // sequence number 
};

// ip header
struct ipheader
{
    unsigned char       iph_length:4;  // IP header length
  unsigned char       iph_ver:4;  //  IP version
    unsigned short int  iph_tos;
  unsigned short int  iph_len; // IP header + data 
    unsigned short int  iph_ident;
 
  unsigned short int  iph_flag:3; // flagmentation flag 
  unsigned short int  iph_offset:13; // flag offset 
    
    unsigned char       iph_ttl;
    unsigned char       iph_protocol;
    unsigned short int  iph_chksum;
    struct in_addr      iph_sourceip;
    struct in_addr      iph_destip;
};

// calculating checksum - used textbook and github resource 
unsigned short in_chksum(unsigned short *buf, int length)
{
    unsigned short *w=buf;
    int nleft=length;
    int sum=0;
    unsigned short temp=0;
    while(nleft>1)
    {
        sum+=*w++;
        nleft-=2;
    }
    if(nleft==1)
    {
        *(u_char *)(&temp)=*(u_char *)w;
        sum+=temp;
    }
    sum=(sum >> 16)+(sum & 0xffff);
    sum+=(sum >> 16);
    return (unsigned short)(~sum);
}

// send raw IP
void send_raw_ip_packet(struct ipheader *ip)
{
    struct sockaddr_in dest_info;
    int enable=1;

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    setsockopt(sock,IPPROTO_IP, IP_HDRINCL, &enable,sizeof(enable));

    // destination info
    dest_info.sin_family=AF_INET;
    dest_info.sin_addr=ip->iph_destip;
   

    // sending raw packet
    sendto(sock,ip,htons(ip->iph_len),0,(struct sockaddr *)&dest_info, sizeof(dest_info));
   
    close(sock);
}

// main
int main()
{
    // buffer of packet
    char buffer[1500];
    memset(buffer,0,1500);
    struct ipheader *ip=(struct ipheader *)buffer;

    //  filling icmp header                     
    struct icmpheader *icmp = (struct icmpheader *)(buffer+sizeof(struct ipheader));
   
    icmp->icmp_type=8;

    // calculate checksum 
    icmp->icmp_chksum=0;
    icmp->icmp_chksum=in_chksum((unsigned short *)icmp, sizeof(struct icmpheader));

   
    // ip header 
    
    ip->iph_ver= 4;
    ip->iph_length= 5;
    ip->iph_ttl= 20;
    ip->iph_sourceip.s_addr=inet_addr("1.2.3.4");
    ip->iph_destip.s_addr=inet_addr("10.0.2.5");
    ip->iph_protocol=IPPROTO_ICMP;
    ip->iph_len=htons(sizeof(struct ipheader)+sizeof(struct icmpheader));

   
    send_raw_ip_packet(ip);
    return 0;
}
