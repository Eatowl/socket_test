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
#define MAX_BUF 256

int main() {
	int server_socket, client_socket;
	struct sockaddr_in saddr;
	char *buf;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	buf = (char *) malloc(MAX_BUF);
	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(3425);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.2");
	if (bind(server_socket, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
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
		while(1) {
            printf("Ожидаем сообщение...\n");
            count = recv(client_socket, buf, 256, 0); // принимаем сообщение от клиента
            if(!strcmp(buf, "exit")) break;
            printf("Получено %d bytes\tСообщение: %s\n", count, buf);
            printf("Отправляю принятое сообщение клиенту\n");
            send(client_socket, buf, count, 0); // отправляем принятое сообщение клиенту
        }
    
        close(client_socket); // закрываем сокет
        _exit(0);
	}
	free(buf);
	close(server_socket);
	//unlink(SOCK_NAME);
	return 0;
}