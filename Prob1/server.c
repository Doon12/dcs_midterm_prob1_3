#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>

extern	int	errno;

#define	BUFSIZE	128
#define	KEY	4507L
#define	DSND  1
#define	CSND  2
#define	FIFOS "/tmp/midterm/fifos.4507"
#define	FIFOC "/tmp/midterm/fifoc.4507"
#define	PERMS 0666

void error(char* msg) {
	fprintf(stderr, "%s error\n", msg);
	exit(1);
}

void Process3(void);
void Process4(void);

typedef struct {
	long type; // message type, must be > 0
	char msgtxt[BUFSIZE]; // message data
} msgbuf;

int MsgQInit (int key) {
	int msgID;
	if (msgID = msgget(key, IPC_CREAT|0666) == -1) {
		perror("msgget error");
		return -1;
	}
	return msgID;
}

int MsgQRcv (int msgID, void* buf, int size, int type) {
	msgbuf rcvbuf;
	int len;
	len = msgrcv(msgID, &rcvbuf, BUFSIZE, type, MSG_NOERROR);
	if (len == -1) {
		return -1;
	}
	memcpy(buf, rcvbuf.msgtxt, size);
	return len;
}

int MsgQSnd (int msgID, void* buf, int size, int type) {
	msgbuf sndbuf;
	sndbuf.type = type;
	memcpy(sndbuf.msgtxt, buf, size);

	if (msgsnd(msgID, &sndbuf, size, IPC_NOWAIT)) {
		perror("MsgASnd");
		return -1;
	}
	return 0;
}

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
		while (1) {
			while(1) {
				if(f_readfd = open(FIFOC, O_RDONLY) == -1){
					error("p2 open FIFOC");
				}
				fifo_len = read(f_readfd, fifo_buf, BUFSIZE);
				if (fifo_len>0){
					printf("3 : Process2 recv: %s\n", fifo_buf);
					close(f_readfd);
					break;
				}
			}

			if(f_writefd = open(FIFOS, O_WRONLY) == -1) {
				error("p2 open FIFOS");
			}
			printf("4 : Process2 send: %s\n", fifo_buf);
			write(f_writefd, fifo_buf, strlen(fifo_buf));
			close(f_writefd);
		}
	}
	else {
		Process3();
	}

	unlink(FIFOS);
	unlink(FIFOC);
	return 0;
}

void Process3(void) {
	int pid = fork();
	if (pid != 0) {
		while (1){
			int f_readfd, f_writefd;
			char fifo_buf[BUFSIZE];
			int fifo_len;

			int MsgID;
			msgbuf buf;
			int MsgType;
			int ret;
			MsgID = MsgQInit(KEY);

			buf.type = CSND;
			ret = MsgQRcv (MsgID, buf.msgtxt, BUFSIZE, buf.type);
			if (ret != -1) {
				if (buf.type == CSND) {
					buf.msgtxt[ret] = '\0';
					printf("1 : Process 3 read from Process 4: %s\n", buf.msgtxt);
				}
			}

			if(f_writefd = open(FIFOC, O_WRONLY) == -1){
				error("p3 open FIFOC");
			}
			printf("2 : Process 3 send: %s\n", buf.msgtxt);

			strncpy(fifo_buf, buf.msgtxt, strlen(buf.msgtxt));
			ret = write(f_writefd, fifo_buf, strlen(fifo_buf));
			printf("write len : %d\n", ret);
			close(f_writefd);

			while(1) {
				memset(fifo_buf, 0, BUFSIZE);
				if(f_readfd = open(FIFOS, O_RDWR) == -1){
					error("p3 open FIFOS");
				}
				fifo_len = read(f_readfd, fifo_buf, BUFSIZE);
				if (fifo_len>0){
					printf("5 : Process 3 recv: %s\n", fifo_buf);
					close(f_readfd);
					break;
				}
				close(f_readfd);
			}

			printf("6 : Process 3 write to Process 4: %s\n", fifo_buf);
			MsgQSnd(MsgID, fifo_buf, strlen(fifo_buf), DSND);
		}

	}
	else {	
		Process4();
	}
}

void Process4(void) {
	int MsgID = MsgQInit(KEY);
	msgbuf mybuf;
	char buf[BUFSIZE];
	int MsgType;
	int ret;

	int i;

	memset(buf, 0, sizeof(buf));
	while(1) {
		i++;
		printf("\n%d -th loop\n",i);
		printf("input: ");
		fgets(buf, BUFSIZE, stdin);
		printf("0 : Process 4 write to Process 3: %s\n", buf);
		MsgQSnd(MsgID, buf, BUFSIZE, CSND);

		mybuf.type = DSND;
		ret = MsgQRcv (MsgID, mybuf.msgtxt, BUFSIZE, mybuf.type);
		if (ret != -1) {
			if (mybuf.type == DSND) {
				printf("7 : Process 4 read from Process 3: %s\n", mybuf.msgtxt);
			}
		}
	}
}
