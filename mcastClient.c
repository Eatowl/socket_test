#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define GROUP "225.0.0.37"
#define MAX_MSG 100

int main() {
     struct sockaddr_in addr;
     int addrlen, sock;
     char message[MAX_MSG];
     if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
          perror("socket error");
          exit(1);
     }
     bzero((char *)&addr, sizeof(addr));
     addr.sin_family = AF_INET;
     addr.sin_addr.s_addr = inet_addr(GROUP);
     addr.sin_port = htons(6000);
     addrlen = sizeof(addr); 
     while (1) {
          fflush(stdin);
          if (!strcmp(fgets(message, MAX_MSG, stdin), "exit")) {
               close(sock);
               return 0;
          }
          printf("sending: %s\n", message);
          if (sendto(sock, message, sizeof(message), 0, (struct sockaddr *) &addr, addrlen) == -1) {
               perror("sendto error");
               close(sock);
               exit(1);
          }
     }
     close(sock);
     return 0;
}
