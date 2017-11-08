#include "unp.h"

// void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
// 	int n;
// 	char sendline[MAXLINE], recvline[MAXLINE + 1];
// 	socklen_t len;
// 	struct sockaddr *preply_addr;
// 	preply_addr = Malloc(servlen);
// 	while (Fgets(sendline, MAXLINE, fp) != NULL) {
// 		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
// 		len = servlen;
// 		n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
// 		// if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0) {
// 		// 	printf("reply from %s (Ignored)\n", Sock_ntop(preply_addr, len));
// 		// 	continue;
// 		// }
// 		recvline[n] = 0;
// 		Fputs(recvline, stdout);
// 	}
// }

// void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
// 	int n;
// 	char sendline[MAXLINE], recvline[MAXLINE + 1];
// 	Connect(sockfd, (SA *)pservaddr, servlen);
// 	while (Fgets(sendline, MAXLINE, fp) != NULL) {
// 		Write(sockfd, sendline, strlen(sendline));
// 		n = Read(sockfd, recvline, MAXLINE);
// 		recvline[n] = 0;
// 		Fputs(recvline, stdout);
// 	}
// }


#define NDG 	2000
#define DGLEN	1400
void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
	int i;
	char sendline[MAXLINE];
	for (i = 0; i < NDG; i ++) {
		Sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
	}
}

int main(int argc, char const *argv[]) {
	int sockfd;
	struct sockaddr_in servaddr;
	if (argc != 2) {
		err_quit("usage:udpcli  <IPaddress>");
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));
	return 0;
}