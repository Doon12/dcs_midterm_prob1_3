#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define	BUFSIZE	1024
#define	PORT	24507
#define	SA	struct	sockaddr

void error_handling(char* message);

char	TERM_MSG[BUFSIZE] = "FINISH";

int main(int argc, char** argv) {
		int sock;
		char message[BUFSIZE];
		int str_len;
		struct sockaddr_in serv_addr;
		struct sockaddr_in clnt_addr;

		// Make here
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == -1) {
				error_handling("TCP Clnt : socket creation");
		}
		else {
				printf("TCP Clnt : socket created\n");
		}

		bzero(&serv_addr, sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serv_addr.sin_port = htons(PORT);

		if (connect(sock, (SA*)&serv_addr, sizeof(serv_addr)) != 0) {
				error_handling("TCP Clnt : connect");
		}
		else {
				printf("TCP Clnt : connect\n");
		}

		while (1) {

				bzero(message, sizeof(message));
				str_len = 0;
				printf("Input : ");
				while((message[str_len++] = getchar()) != '\n');

				printf("TCP Client to Server : %s\n", message);
				write(sock, message, sizeof(message));

				if (strcmp(message, TERM_MSG) == 0) {
						printf("TCP Client exit\n");
						break;
				}
				
				// echo
				bzero(message, BUFSIZE);

				read(sock, message, sizeof(message));
				printf("TCP Client from Server : %s\n", message);

		}

		close (sock);

		return 0;
}

void error_handling(char* message) {
		fputs(message, stderr);
		fputc('\n', stderr);
		exit(1);
}
