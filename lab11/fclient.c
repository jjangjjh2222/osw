#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define DEFAULT_PROTOCOL 0
#define BUF_SIZE 100

int readLine(int, char*);

int main(int argc, char *argv[]) {
	int sfd, port, result;
	char *host, msg_recv[BUF_SIZE], msg_send[BUF_SIZE];
	struct sockaddr_in serverAddr;
	struct hostent *hp;

	if( argc != 3 ) {
		fprintf(stderr, "Usage: %s <host> <port> \n", argv[0]);
		exit(0);
	}
	
	host = argv[1];  /* code here */
	port = atoi(argv[2]); /* code here */

	sfd = socket(PF_INET, SOCK_STREAM, DEFAULT_PROTOCOL); /* code here */

	if( (hp = gethostbyname(host)) == NULL ) {
		perror("gethostbyname error");
	}

	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	bcopy((char *) hp->h_addr_list[0], (char *) &serverAddr.sin_addr.s_addr, hp->h_length);
	serverAddr.sin_port = htons(port);

	do {
		result = connect(sfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); /* code here */
		if ( result == -1 ) {
			sleep(1);
		}
	} while ( result == -1 );

	printf("다운로드할 파일 이름 입력:");
	scanf("%s", msg_recv);
	write(sfd, msg_recv, strlen(msg_recv) + 1);
	FILE *fp = fopen(msg_recv, "r");
	while ((fgets(msg_send, BUF_SIZE, fp)) != NULL) { /* code here */
		printf("%s", msg_send);
	}
	close(sfd);
	exit(0);
}

int readLine(int fd, char* str) {
	int n;
	do {
		 n = read(fd, str, 1);
	} while( n>0 && *str++ != '\0' );

	return ( n>0 );
}
