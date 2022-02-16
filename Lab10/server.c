#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

void* client_t(void* param)
{
	int* args = (int*)param;
	printf("%d %d\n", args[0], args[1]);
	int receive_fd = args[0];
	int send_fd = args[1];
	printf("receive_fd %d send_fd %d\n", receive_fd, send_fd);
	int tnum;
	if(send_fd>receive_fd) tnum=1;
	else tnum=2;

	// printf("Thread %d sixth\n", tnum);
	// printf("Thread %d seventh\n", tnum);
	int n;

	char buffer[BUFFER_SIZE];
	// printf("Thread %d eighth\n", tnum);

	while(1)
	{
		//RECEIVE STRING FROM ONE CLIENT//
		memset(buffer, 0, sizeof(buffer));
		// printf("Thread %d : Waiting to read\n", tnum);
		if(read(receive_fd, buffer, sizeof(buffer))<0)
		{
			printf("Thread %d: Error reading\n", tnum);
			break;
		}
		else 
		{
			if(strlen(buffer)>0) printf("Thread %d: Received\n", tnum);
			else break;
		}
	    //SEND STRING TO OTHER CLIENT//
        n = strlen(buffer);
        if((write(send_fd, buffer, n))<=0) printf("Thread %d: Error sending\n", tnum);
        else printf("Thread %d: Forwarded\n\n", tnum);
	}
	close(receive_fd);
	pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock_fd<0)
	{
		printf("Socket failed\nExiting\n");
		exit(1);
	}
	else printf("Socket created\n");

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));

	int port_no = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	// inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port_no);


	if(bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
	{
		printf("Unable to bind\n");
		exit(1);
	}
	else printf("Socket bound\n");

	if(listen(sock_fd, 5))
	{
		printf("Unable to listen\n");
		exit(1);
	}
	else printf("Socket listening\n");

	// printf("assigned\n");

	pthread_t tid[2];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	// pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	struct sockaddr_in cli_addr1, cli_addr2;
	int len = sizeof(cli_addr1);

	int accept_fd1, accept_fd2;

	while(1)
	{
		if((accept_fd1 = accept(sock_fd, (struct sockaddr*)&(cli_addr1), &len))<0)
		{
			printf("Client 1 falied to connect\n");
			exit(1);
		}
		else 
		{
			printf("Client 1 connected\n");
		}
		
		if((accept_fd2 = accept(sock_fd, (struct sockaddr*)&(cli_addr2), &len))<0)
		{
			printf("Client 2 falied to connect\n");
			exit(1);
		}
		else 
		{
			printf("Client 2 connected\n");
		}

		int ptr1[2];
		int ptr2[2];

		ptr1[0]=accept_fd1;
		ptr1[1]=accept_fd2;
		pthread_create(&(tid[0]), &attr, client_t, ptr1);
		// printf("Thread 1 created\n");

		ptr2[0]=accept_fd2;
		ptr2[1]=accept_fd1;
		pthread_create(&(tid[1]), &attr, client_t, ptr2);

		pthread_join(tid[0], NULL);
		pthread_join(tid[1], NULL);
		// printf("Both clients have exited\n");
	}

	close(sock_fd);
	return 0;
}