#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

extern	int	errno;

#define	BUFSIZE	128
#define	KEY	4507L
#define	DSND  1
#define	CSND  2
#define	FIFOS "/tmp/midterm/fifos"
#define	FIFOC "/tmp/midterm/fifoc"
#define	PERMS 0666

void error(char* msg) {
		fprintf(stderr, "%s error\n", msg);
		exit(1);
}

void Process3(void);

int main(void) {
		int f_readfd, f_writefd;
		char fifo_buf[BUFSIZE];
		int fifo_len;
		if(mkfifo(FIFOS,  PERMS)<0) {
				error("can't create fifo S");
		}
		if(mkfifo(FIFOC,  PERMS)<0) {
				error("can't create fifo C");
		}



		memset(fifo_buf, 0, BUFSIZE);

		int pid = fork();
		if (pid != 0) {
				if(f_readfd = open(FIFOC, O_RDONLY) == -1){
						error("p2 open FIFOC");
				}
				printf("readfd : %d\n", f_readfd);
				while(fifo_len = read(f_readfd, fifo_buf, BUFSIZE) > 0){
						printf("fifo len : %d\n", fifo_len);
						fifo_buf[fifo_len] = '\0';
						printf("Process2 recv: %s\n", fifo_buf);
				}
				close(f_readfd);
		}
		else {
				Process3();
		}

		unlink(FIFOS);
		unlink(FIFOC);
		return 0;
}

void Process3(void) {
		printf("P3 start\n");
		int f_readfd, f_writefd;
		char fifo_buf[BUFSIZE];
		int fifo_len;
		int ret;


		if(f_writefd = open(FIFOC, O_WRONLY) == -1){
				error("p3 open FIFOC");
		}
		
		printf("input : ");

		while(fgets(fifo_buf, sizeof(fifo_buf), stdin), !feof(stdin)) {
				if (ret = write(f_writefd, fifo_buf, strlen(fifo_buf)+1) < 0) {
						printf("write len : %d\n", ret);
				} else {
						printf("writefd : %d\n", f_writefd);
						printf("Process 3 send: %s\n", fifo_buf);
				}
		}
		close(f_writefd);
}
