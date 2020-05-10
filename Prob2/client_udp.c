//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define  BUFSIZE   1024
#define	PORT	24507


void error_handling(char* );

int   main( void)
{
	int   sock;
	int   serv_addr_size;

	struct sockaddr_in   serv_addr;

	char   recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];


	sock  = socket( PF_INET, SOCK_DGRAM, 0);
	if( -1 == sock)
		{
			error_handling("UDP Client : socket");
		}

	memset( &serv_addr, 0, sizeof( serv_addr));
	serv_addr.sin_family     = AF_INET;
	serv_addr.sin_port       = htons( PORT);
	serv_addr.sin_addr.s_addr= inet_addr( "127.0.0.1");

	while (1) {
			memset(send_buf, 0, BUFSIZE);
			printf("Input : ");
			scanf("%s", send_buf);


			sendto( sock, send_buf, strlen(send_buf)+1, 0,
						 ( struct sockaddr*)&serv_addr, sizeof( serv_addr));          

			if (strcmp(send_buf, "q") == 0)
				break;
			serv_addr_size  = sizeof( serv_addr);
			recvfrom( sock, recv_buf, BUFSIZE, 0 , 
							 ( struct sockaddr*)&serv_addr, &serv_addr_size);
			printf( "receive: %s\n", recv_buf);
	}
	close( sock);

	return 0;
}


void error_handling(char* message) {
		fputs(message, stderr);
		fputc('\n', stderr);
		exit(1);
}
