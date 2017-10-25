#include "unp.h"

struct args {
	long arg1;
	long arg2;
};

struct result {
	long sum;
};

void str_echo (int sockfd) {
	size_t n;
	struct args args;
	struct result result;
	for (;;) {
		if ((n = Readn(sockfd, &args, sizeof(args))) == 0) {
			return;
		}
		result.sum = args.arg1 + args.arg2;
		Writen(sockfd, &result, sizeof(result));
	}
}

void sig_chld(int signo) {
	pid_t pid;
	int stat;

	// pid = wait(&stat);
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		printf("child %d terminated\n", pid);
	}
}

int main(int argc, char const *argv[]) {
	int listenfd, connfd;
	pid_t childpid;
	socklen_t chilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	Signal(SIGCHLD, sig_chld);

	for (;;) {
		chilen = sizeof(cliaddr);
		if ((connfd = accept(listenfd, (SA *)&cliaddr, &chilen)) < 0) {
			if (errno == EINTR) {
				printf("einter\n");
				continue;
			} else {
				err_sys("accept error");
			}
		} 
		if ((childpid = Fork()) == 0) {
				Close(listenfd);
				str_echo(connfd);
				Close(connfd);
				exit(0);
			}
			Close(connfd);
	}

	return 0;
}