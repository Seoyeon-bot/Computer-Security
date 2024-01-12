#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>


// ip header
struct ipheader
{
   
    unsigned char       iph_length:4, iph_ver:4;
    unsigned char       iph_tos;
    unsigned short int  iph_len;
   
    unsigned short int  iph_ident;
    unsigned short int  iph_flag:3;
    unsigned short int  iph_offset:13;
    unsigned char       iph_ttl;
   
    unsigned char       iph_protocol;
    unsigned short int  iph_chksum;
    struct in_addr      iph_sourceip;
    struct in_addr      iph_destip;
};

// calculate  checksum
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

  
    dest_info.sin_family=AF_INET;
    dest_info.sin_addr=ip->iph_destip;
    dest_info.sin_port = htons(9090);

    // sending raw packet
    sendto(sock,ip,htons(ip->iph_len),0,(struct sockaddr *)&dest_info, sizeof(dest_info));

    close(sock);
}

// main
int main()
{
  // create buffer 
    char buffer[1500];
    memset(buffer,0,1500);
    struct ipheader *ip=(struct ipheader *)buffer;
    
    // cerate ip header infos 
    ip->iph_ver=4;
    ip->iph_length=5;
    ip->iph_ttl= 20;
    ip->iph_sourceip.s_addr=inet_addr("10.2.0.15");
    ip->iph_destip.s_addr=inet_addr("10.2.0.5");
    ip->iph_protocol=IPPROTO_RAW;
    ip->iph_len = sizeof(struct ipheader);

    // send raw packet 
    send_raw_ip_packet(ip);
   
    return 0;
}
