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
	int   clnt_addr_size;

	struct sockaddr_in   serv_addr;
	struct sockaddr_in   clnt_addr;

	char   recv_buf[BUFSIZE];
	char   send_buf[BUFSIZE];

	sock  = socket(PF_INET, SOCK_DGRAM, 0);

	if( -1 == sock)
		{
			error_handling("UDP Serv: Socket");
			
		}

	memset( &serv_addr, 0, sizeof( serv_addr));
	serv_addr.sin_family     = AF_INET;
	serv_addr.sin_port       = htons( PORT);
	serv_addr.sin_addr.s_addr= htonl( INADDR_ANY);

	if( -1 == bind( sock, (struct sockaddr*)&serv_addr, sizeof( serv_addr) ) )
		{
			error_handling("UDP Serv: bind");
		}

	while( 1)
		{
			clnt_addr_size  = sizeof( clnt_addr);
			recvfrom( sock, recv_buf, BUFSIZE, 0 , 
							 ( struct sockaddr*)&clnt_addr, &clnt_addr_size);
			if (strcmp(recv_buf, "q") == 0)
				break;
			printf( "receive: %s\n", recv_buf);

			sprintf( send_buf, "%s", recv_buf);
			sendto( sock, send_buf, strlen( send_buf)+1, 0,  // +1: NULL까지 포함해서 전송
						 ( struct sockaddr*)&clnt_addr, sizeof( clnt_addr)); 
		}
}

void error_handling(char* message) {
		fputs(message, stderr);
		fputc('\n', stderr);
		exit(1);
}
