#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#define PORT_TIME 10

int main() {
	struct sockaddr_in dest;
	char *host = "127.0.0.1";
	int sd;
	sd = socket(PF_INET, SOCK_STREAM, 0);
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(PORT_TIME);
	inet_aton(host, dest.sin_addres);
	if (connect(sd, &dest, sizeof(dest)) != 0) {
		perror("socket connection");
		abort();
	}
	return 0;
}