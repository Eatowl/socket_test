#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <resolv.h>
#include <string.h>

#define PORT_TIME 10
#define MAX_BUF 128
#define HOST "127.0.0.1"

int main() {
	struct sockaddr_in dest;
	char *host = "127.0.0.1";
	char buffer[MAX_BUF];
	int sd;
	sd = socket(PF_INET, SOCK_STREAM, 0);
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(PORT_TIME);
	inet_aton(host, &dest.sin_addr);
	if (connect(sd, (struct sockaddr_in *) &dest, sizeof(dest)) != 0) {
		perror("socket connection failed");
		abort();
	}
	fflush(stdin);
	fgets(buffer, MAX_BUF, stdin);
	//sprintf(buffer, "%s\n", MAX_BUF);
	send(sd, buffer, strlen(buffer), 0);
	bzero(buffer, MAX_BUF);
	recv(sd, buffer, MAX_BUF - 1, 0);
	printf("%s\n", buffer);
	close(sd);
	return 0;
}