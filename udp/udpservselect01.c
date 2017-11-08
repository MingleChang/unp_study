#include "unp.h"

void str_echo(int sockfd) {
	size_t n;
	char line[MAXLINE];
	for (;;) {
		if ((n = Readline(sockfd, line, MAXLINE)) == 0) {
			return;
		}
		printf("%s\n", line);
		Writen(sockfd, line, n);
	}
}

void sig_child(int signo) {
	pid_t pid;
	int stat;

	// pid = wait(&stat);
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		printf("child %d terminated\n", pid);
	}
}

int main(int argc, char const *argv[]) {
	int listenfd, connfd, udpfd, nready, maxfdp1;
	char mesg[MAXLINE];
	pid_t childpid;
	fd_set rset;
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;

	void sig_child(int);
	
	//Create TCP Socket
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);

	//Create UDP Socket
	udpfd = Socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(udpfd, (SA *)&servaddr, sizeof(servaddr));

	Signal(SIGCHLD, sig_child);

	FD_ZERO(&rset);
	maxfdp1 = max(listenfd, udpfd) + 1;
	for (;;) {
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
		if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
			if (errno == EINTR) {
				continue;
			}else {
				err_sys("select error");
			}
		}
		if (FD_ISSET(listenfd, &rset)) {
			len = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *)&cliaddr, &len);
			if ((childpid = Fork()) == 0) {
				Close(listenfd);
				str_echo(connfd);
				Close(connfd);
				exit(0);
			}
			Close(connfd);
		}
		if (FD_ISSET(udpfd, &rset)) {
			len = sizeof(cliaddr);
			n = Recvfrom(udpfd, mesg, MAXLINE, 0, (SA *)&cliaddr, &len);
			Sendto(udpfd, mesg, n, 0, (SA *)&cliaddr,  len);
		}
	}

	return 0;
}