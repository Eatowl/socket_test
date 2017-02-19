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
#include <sys/epoll.h>

#define QUEUE_LENGTH 5
#define MAX_BUF 	 256
#define MAX_EVENTS   10

int main() {
	int server_socket, client_socket;
	struct sockaddr_in saddr;
	char *buf;
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	buf = (char *) malloc(MAX_BUF);
	bzero(&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(3425);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.2");
	if (bind(server_socket, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
		perror("bind() error");
		exit(1);
	}
	if (listen(server_socket, QUEUE_LENGTH) == -1) {
		perror("listen() error");
		exit(1);
	}
	struct epoll_event ev, events[MAX_EVENTS];
	int count, nfds, epollfd;
	if ((epollfd = epoll_create1(0)) == -1) {
		perror("epoll_create");
		exit(1);
	}
	ev.events = EPOLLIN;
	ev.data.fd = server_socket;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_socket, &ev) == -1) {
		perror("epoll_ctl: server_socket");
		exit(1);
	}
	while (1) {
		if ((nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) == -1)
			perror("epoll_wait");
		for (int i = 0; i < nfds; ++i) {
			if (events[i].data.fd == server_socket) {
				if ((client_socket = accept(server_socket, NULL, NULL)) == -1) {
					perror("accept error");
					exit(1);
				}
				ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
				ev.data.fd = client_socket;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_socket, &ev) == -1) {
					perror("epoll_ctl: server_socket");
					exit(1);
					}
			} else {
				if (events[i].events & EPOLLIN) {
					printf("-> event=%lu on fd=%d\n",
						(unsigned long)events[i].events,
										events[i].data.fd);
					if ((count = recv(events[i].data.fd, buf, MAX_BUF, 0)) == -1)
						perror("recv error");
					printf("->>%s\n", buf);
					if (send(events[i].data.fd, buf, count, 0) == -1)
						perror("send error");
				}
			}
		}
	}
	free(buf);
	close(server_socket);
	return 0;
}
