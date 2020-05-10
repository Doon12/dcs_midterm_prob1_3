#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char** argv) {
		int listenfd, connfd;
		struct sockaddr_in cliaddr, servaddr;
		socklen_t clilen;
		int serverNumber;
		char intro[] = "Input Number (1 ~ 100) : ";
		char more[] = "The number is more than the input number.\n";
		char less[] = "The number is less than the input number.\n";
		char same[] = "You are right.\n";
		if (argc != 2) {
				printf("Usage : %s <port> \n", argv[0]);
				exit(1);
		}

		// Part A
		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		if (listenfd == -1) {
				printf("socket createt fail\n");
				return 0;
		}

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(atoi(argv[1]));

		// Part B
		if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
				printf("bind error\n");
				return 0;
		}
		if(listen(listenfd, 5) < 0) {
				printf("listen error\n");
				return 0;
		}
		printf("ready game...\n");
		while ( connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) {
				// Connection now accepted
				int pid;
				pid = fork();
				if (pid == 0) {
						/* If parent, close connection and accept again. */
						printf("Connection Established\n");
						close(connfd);
				}
				else {
						write(connfd, intro, sizeof(intro));
						srand((unsigned)time(NULL));
						serverNumber = rand() % (1 + 99) + 1; // 1 ~ 100
						printf("server set number : %d\n", serverNumber);
						while(1) {
								char buffer[1024];
								bzero(buffer, sizeof(buffer));
								int clientNumber = 0;

								// Part C
								int read_len;
								read_len = read(connfd, buffer, sizeof(buffer));
								clientNumber = atoi(buffer);
								printf("client input number : %d\n", clientNumber);
								if(serverNumber > clientNumber) {
										sprintf(buffer, "%s%s", more, intro);
										// Part D
										write(connfd, buffer, sizeof(buffer));
								}
								else if (serverNumber < clientNumber) {
										sprintf(buffer, "%s%s", less, intro);
										// Part E
										write(connfd, buffer, sizeof(buffer));
								}
								else {
										// Part F
										sprintf(buffer, "%s", same);
										write(connfd, buffer, sizeof(buffer));
										break;
								}
						}
						printf("Client answers the number.\nThe end\n");
						close(connfd);
				}
		}
		close(listenfd);

		return 0;
}
