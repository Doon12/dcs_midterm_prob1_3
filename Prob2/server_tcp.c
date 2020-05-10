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
		int serv_sock;
		int clnt_sock;
		char message[BUFSIZE];
		int str_len;
		struct sockaddr_in serv_addr;
		struct sockaddr_in clnt_addr;

		// Make here
		serv_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (serv_sock == -1) {
				error_handling("TCP Serv : socket creation");
		}
		else {
				printf("TCP Serv : socket created\n");
		}

		bzero(&serv_addr, sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(PORT);

		if ((bind(serv_sock, (SA*)&serv_addr, sizeof(serv_addr))) != 0) {
				error_handling("TCP Serv : socket bind");
		}
		else {
				printf("TCP Serv : bind\n");
		}

		if ((listen(serv_sock, 5)) != 0) {
				error_handling("TCP Serv : socket listen");
		}
		else {
				printf("TCP Serv : listen\n");
		}

		str_len = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (SA*)&clnt_addr, &str_len);
		if (clnt_sock < 0) {
				error_handling("TCP Serv : accept");
		}
		else {
				printf("TCP Serv : accept\n");
		}

		// echo

		while (1) {

				bzero(message, BUFSIZE);

				read(clnt_sock, message, sizeof(message));
				
				if (strcmp(message, TERM_MSG) == 0) {
						printf("TCP Server exit\n");
						break;
				}

				printf("TCP Server from Client : %s\n", message);

				printf("TCP Server to Client : %s\n", message);
				write(clnt_sock, message, sizeof(message));

				
				
		}

		close (clnt_sock);

		close (serv_sock);

		return 0;
}

void error_handling(char* message) {
		fputs(message, stderr);
		fputc('\n', stderr);
		exit(1);
}
