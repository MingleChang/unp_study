#include "unp.h"

// void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen) {
// 	int n;
// 	socklen_t len;
// 	char msg[MAXLINE];
// 	for (;;) {
// 		len = clilen;
// 		n = Recvfrom(sockfd, msg, MAXLINE, 0, pcliaddr, &len);
// 		Sendto(sockfd, msg, n, 0, pcliaddr, len);
// 	}
// }

static void recvfrom_int(int);
static int count;
void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen) {
	socklen_t len;
	char msg[MAXLINE];
	Signal(SIGINT, recvfrom_int);
	for (;;) {
		len = clilen;
		Recvfrom(sockfd, msg, MAXLINE, 0, pcliaddr, &len);
		count ++;
	}
}
static void recvfrom_int(int signo) {
	printf("\nreceived %d dategrams\n", count);
	exit(0);
}

int main(int argc, char const *argv[]) {
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(sockfd, (SA *)&servaddr, sizeof(servaddr));
	dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));
	return 0;
}