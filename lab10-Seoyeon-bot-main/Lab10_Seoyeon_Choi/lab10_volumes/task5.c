

#include <pcap.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <net/ethernet.h>

struct ethheader{
	u_char ether_dhost[ETHER_ADDR_LEN];
	u_char ether_shost[ETHER_ADDR_LEN];
	u_short ether_type;
};

struct ipheader {
	unsigned char iph_ihl:4, iph_ver:4; // IP haeder lenght 4 ip version length 4 
	unsigned char iph_tos; // type of servie 
	unsigned short int iph_len;  // ip packet length ( header + data)
	unsigned short int iph_ident;   // identification
	unsigned short int iph_flag:3, iph_offset:13;  // fragmentation flage s flages offset
	unsigned char iph_ttl; // tiem to live 
	unsigned char iph_protocol; // ip protocol type 
	unsigned short int iph_chksum; //ip checksum
	struct in_addr iph_sourceip; 	// source ip address 
	struct in_addr iph_destip;  // desitnation ip addr 
};

// TCP header 
typedef u_int tcp_seq;

struct tcpheader {
	u_short th_sport;	/* source port */
	u_short th_dport;	/* destination port */
	tcp_seq th_seq;		/* sequence number */
	tcp_seq th_ack;		/* acknowledgement number */
	u_char th_offx2;	/* data offset, rsvd */
	#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
	u_char th_flags;
	#define TH_FIN 0x01
	#define TH_SYN 0x02
	#define TH_RST 0x04
	#define TH_PUSH 0x08
	#define TH_ACK 0x10
	#define TH_URG 0x20
	#define TH_ECE 0x40
	#define TH_CWR 0x80
	#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
	u_short th_win;		/* window */
	u_short th_sum;		/* checksum */
	u_short th_urp;		/* urgent pointer */
};

//this is from lecture slide 17.  -> check wehther packet were received. 
void got_packet(u_char *args, const struct pcap_pkthdr *header,const u_char *packet)
{	char *data;
	int i, size_tcp;
	printf("Received a packet\n"); 
	
	struct ethheader *eth = (struct ethheader *)packet;
	
	// 0x0800 :ip header type 
	if (ntohs(eth->ether_type) == 0x0800){
		struct ipheader * ip = (struct ipheader *)(packet + sizeof(struct ethheader));
		int ip_header_len = ip->iph_ihl * 4; //?
		struct tcpheader *tcp = (struct tcpheader *)((u_char *)ip + ip_header_len);
		
	
		
		size_tcp = TH_OFF(tcp)*4;
		data = (u_char *)(packet + 14 + ip_header_len + size_tcp);
		printf("%s",data);
		
		printf("From : %s\n", inet_ntoa(ip->iph_sourceip)); 
		printf("To: %s\n", inet_ntoa(ip->iph_destip));
		
		

	}
}

int main(){
pcap_t *handle;
char errbuf[PCAP_ERRBUF_SIZE];
struct bpf_program fp;
// for task2.1 
// char filter_exp[] = "icmp";

// Task2.1B • Capture the ICMP packets between two specific hosts.
//char filter_exp[] = "icmp and src host 10.0.2.15 and dst host 8.8.8.8";  // 

// Task2.1B capture TCP packets with a destination port number in the range from 10 to 100.
//char filter_exp[] ="tcp and dst portrange 10-100";s

// Task 2.1C: Sniffing Passwords.
char filter_exp[] = "tcp"; // listen port 23

bpf_u_int32 net;

// Step 1: Open live pcap session on NIC with name enp0s3
handle = pcap_open_live("enp0s3", BUFSIZ, 1, 1000, errbuf);  

// Step 2: Compile filter_exp into BPF psuedo-code
pcap_compile(handle, &fp, filter_exp, 0, net);
pcap_setfilter(handle, &fp);

if (pcap_setfilter(handle, &fp) !=0) {
pcap_perror(handle, "Error:");
    exit(EXIT_FAILURE);
}


// Step 3: Capture packets
pcap_loop(handle, -1, got_packet, NULL);
pcap_close(handle); //Close the handle


return 0;
}

