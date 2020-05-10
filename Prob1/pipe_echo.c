#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define	BUFSIZE	1024

int main(void) {

		int fd1[2], nbytes1;
		int fd2[2], nbytes2;
		pid_t childpid;
		char* string1;
		char* string2;
		char readbuffer1[BUFSIZE];
		char readbuffer2[BUFSIZE];
		
		pipe(fd1);
		pipe(fd2);

		childpid = fork();
		if (childpid == 0) {
				// child
				while (1) {
						printf("Input : ");
						scanf("%s", string1);
						close(fd1[0]);
						write(fd1[1], string1, strlen(string1)+1);
						if (strcmp(string1, "q") == 0)
							break;

						close(fd2[1]);
						nbytes1 = read(fd2[0], readbuffer1, sizeof(readbuffer1));
						printf("Child Received String : %s\n", readbuffer1);
				}
		}
		else {
				// parent
				while (1) {
						close(fd1[1]);
						nbytes2 = read(fd1[0], readbuffer2, sizeof(readbuffer2));
						if (strcmp(readbuffer2, "q") == 0)
							break;

						printf("Parent Received String : %s\n", readbuffer2);

						strcpy(string2, readbuffer2); 

						close(fd2[0]);
						write(fd2[1], string2, strlen(string2)+1);
				}

		}

		return 0;
}
