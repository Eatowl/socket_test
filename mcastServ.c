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
    int s_sock;
    socklen_t addrlen;
    struct ip_mreq mreq;
    char message[MAX_MSG];
    if ((s_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(6000);
    addrlen = sizeof(addr);
    if (bind(s_sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {        
        perror("bind error");
        close(s_sock);
        exit(1);
    }    
    mreq.imr_multiaddr.s_addr = inet_addr(GROUP);         
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);         
    if (setsockopt(s_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt mreq");
        close(s_sock);
        exit(1);
    }         
    while (1) {
        if (recvfrom(s_sock, message, sizeof(message), 0, (struct sockaddr *) &addr, &addrlen) == -1) {
            perror("recvfrom error");
            close(s_sock);
            exit(1);
        }
        printf("%s: message = %s\n", inet_ntoa(addr.sin_addr), message);
    }
    close(s_sock);
    return 0;
}