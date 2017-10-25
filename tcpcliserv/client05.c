#include "unp.h"

struct args {
	long arg1;
	long arg2;
};

struct result {
	long sum;
};

void str_cli(FILE *fp, int sockfd) {
	char sendline[MAXLINE];
	struct args args;
	struct result result;
	while (Fgets(sendline, MAXLINE, fp) != NULL) {
		if (sscanf(sendline, "%ld%ld", &args.arg1, &args.arg2) != 2) {
			printf("invalid input: %s\n", sendline);
			continue;
		}
		Writen(sockfd, &args, sizeof(args));
		if (Readn(sockfd, &result, sizeof(result)) == 0) {
			err_quit("str_cli:server terminated prematurely");
		}
		printf("%ld\n", result.sum);
	}
}

int main(int argc, char const *argv[]) {	
	int sockfd;
	struct sockaddr_in servaddr;

	if (argc != 2) {
		err_quit("usage: client <IPAddress>");
	}
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1],&servaddr.sin_addr);
	Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
	str_cli(stdin, sockfd);

	return 0;
}