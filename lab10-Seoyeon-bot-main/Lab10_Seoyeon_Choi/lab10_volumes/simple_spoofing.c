#include <stdio.h>
#include <stdlib.h> // for exit
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Mainly used lecture slide 21 example code. 
void main()

{

 struct sockaddr_in dest_info;

 char *data = "Sending UDP message\n";

 // create socket with SOCK_DGRAM for UDP> 

 int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

 // dest info, address,port# 

 memset((char *) &dest_info, 0, sizeof(dest_info));

 dest_info.sin_family = AF_INET;

 dest_info.sin_addr.s_addr = inet_addr("10.0.2.5");

 dest_info.sin_port = htons(9090);

 // sending packet

 sendto(sock, data, strlen(data), 0,(struct sockaddr *)&dest_info, 

sizeof(dest_info));

 // closing socket

 close(sock);
 }
