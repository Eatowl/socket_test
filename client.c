#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <resolv.h>
#include <string.h>
#include <stdio.h>
#include <sys/un.h>

#define PORT_TIME 10
#define MAX_BUF 128
#define SOCK_NAME "mysocket1"

int main() {
	char buffer[MAX_BUF];
	int cl_sock;
	struct sockaddr_un saddr;
	cl_sock = socket(PF_UNIX, SOCK_STREAM, 0);
	bzero(&saddr, sizeof(saddr));
	saddr.sun_family = AF_UNIX;
	strcpy (saddr.sun_path, SOCK_NAME);
	if (connect(cl_sock, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
		perror("socket connection failed");
		return 1;
	}
	while (1) {
		fflush(stdin);
		fgets(buffer, MAX_BUF, stdin);
		//sprintf(buffer, "%s\n", MAX_BUF);
		write(cl_sock, buffer, strlen(buffer));
		//if (!strcmp(buffer, "exit"))
		//	break;
		bzero(buffer, MAX_BUF);
	}	
	close(cl_sock);
	return 0;
}