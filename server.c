#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <resolv.h>
#include <string.h>

#define MAX_BUF 128

int main() {
	int sd;
	char *buf;
	sd = socket(PF_INET, SOCK_STREAM, 0);
	buf = (char *) malloc(MAX_BUF);
	
	return 0;
}