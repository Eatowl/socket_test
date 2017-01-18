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

#define QUEUE_LENGTH 12
#define MAX_BUF 128
#define SOCK_NAME "mysocket1"

int main() {
	int server_socket, client_socket;
	struct sockaddr_un saddr;
	char *buf;
	server_socket = socket(PF_UNIX, SOCK_STREAM, 0);
	buf = (char *) malloc(MAX_BUF);
	bzero(&saddr, sizeof(saddr));
	saddr.sun_family = AF_UNIX;
	strcpy (saddr.sun_path, SOCK_NAME);
	if (bind(server_socket, (struct sockaddr *) &saddr, SUN_LEN(&saddr)) == -1) {
		perror("bind() error");
		return 1;
	}
	if (listen(server_socket, QUEUE_LENGTH) == -1) {
		perror("listen() error");
		return 0;
	}
	int count;
	while(1) {
		client_socket = accept(server_socket, NULL, NULL);
		count = read(client_socket, buf, MAX_BUF - 1);
		buf[count] = '\0';
		printf(">> %s\n", buf);
		close(client_socket);
		if (!strcmp(buf, "exit"))
			break;
	}
	free(buf);
	close(server_socket);
	unlink(SOCK_NAME);
	return 0;
}