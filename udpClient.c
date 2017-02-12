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
#define MAX_BUF 256

int main() {
	char buffer[MAX_BUF];
	char message[MAX_BUF];
	int cl_sock;
	struct sockaddr_in saddr;
	cl_sock = socket(PF_INET, SOCK_DGRAM, 0);
	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(3425);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(cl_sock, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
		perror("socket connection failed");
		close(cl_sock);
		return 1;
	}
	while (1) {
		fflush(stdin);
		if (!strcmp(fgets(buffer, MAX_BUF, stdin), "exit")) {
			close(cl_sock);
			return 0;
		}
		printf("отправка сообщения на сервер...\n");
		if (send(cl_sock, buffer, sizeof(buffer), 0) == -1) {
			perror("send() error");
			close(cl_sock);
			exit(1);
		}
		int bytes_read = 0;
		printf("Ожидание сообщения\n");
		if ((bytes_read = recv(cl_sock, message, sizeof(buffer), 0)) == -1) {
			perror("recv() error");
			close(cl_sock);
			exit(1);
		}
		printf("Получено %d bytes\tСообщение: %s\n", bytes_read, message);
		bzero(buffer, MAX_BUF);
		bzero(message, MAX_BUF);
	}	
	close(cl_sock);
	return 0;
}
