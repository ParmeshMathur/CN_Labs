#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 200

int sock_fd;
int cli_num;
int running[4];

void* runner(void* param)
{
	int* args = (int*)param;
	int index = args[0];
	int accept_fd = args[1];

	printf("Clients connected: %d\n", cli_num);

	struct sockaddr_in cli_addr;
	int len = sizeof(cli_addr);

	char buffer[BUFFER_SIZE];

	while(1)
	{
		//RECEIVE STRING FROM CLIENT//
		memset(buffer, 0, sizeof(buffer));
		read(accept_fd, buffer, sizeof(buffer));	
		if(strcmp(buffer, "exit")==0)
		{
			printf("Client has exited\n\n");
			break;
		}

		//REVERSE STRING//
		int n = strlen(buffer);
        for (int i=0;i<n/2;i++)
        {
        	char temp = buffer[i];
        	buffer[i]=buffer[n-i-1];
        	buffer[n-i-1]=temp;
        }
        printf("%s\n", buffer);

        //TAKE INPUT STRING//
        memset(buffer, 0, sizeof(buffer));
        printf("Enter string: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        n = strlen(buffer);
        buffer[n-1] = '\0';

        // printf("string scanned\n");
        if(strcmp(buffer,"exit")==0)
	    {
	        n = strlen(buffer);
	        write(accept_fd, buffer, n);

	        printf("\nExiting\n");
	        break;
	    }

	    //SEND MESSAGE TO CLIENT IF NOT EXIT STRING//
        n = strlen(buffer);
        write(accept_fd, buffer, n);
        // printf("Sending message to client.\n");
        printf("\n");
	}
	close(accept_fd);
	cli_num--;
	running[index]=0;
	pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	cli_num=0;
	sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock_fd<0)
	{
		printf("Socket failed\nExiting\n");
		exit(0);
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
		exit(0);
	}
	else printf("Socket bound\n");

	if(listen(sock_fd, 4))
	{
		printf("Unable to listen\n");
		exit(0);
	}
	else printf("Socket listening\n");

	// printf("assigned\n");

	pthread_t tid[4];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	for(int i=0;i<4;i++) running[i]=0;

	struct sockaddr_in cli_addr;
	int len = sizeof(cli_addr);

	while(1)
	{
		if(cli_num<4)
		{
			int accept_fd = accept(sock_fd, (struct sockaddr*)&(cli_addr), &len);
			if(accept_fd>=0)
			{
				for(int i=0;i<4;i++)
				{
					if(running[i]==0)
					{
						write(accept_fd, "stay", strlen("stay"));
						printf("Socket connected, server accepting\n");
						int* ptr = (int*)malloc(2*sizeof(int));
						ptr[0]=i;
						ptr[1]=accept_fd;
						// i = 0;
						cli_num++;
						running[0]=1;
						pthread_create(&(tid[i]), &attr, runner, ptr);
						break;
					}
				}
			}
		}
		else if(cli_num==4)
		{
			int accept_fd = accept(sock_fd, (struct sockaddr*)&(cli_addr), &len);
			if(accept_fd>=0)
			{
				write(accept_fd, "exit", strlen("exit"));
				close(accept_fd);
			}
			continue;
		}
	}

	close(sock_fd);
	return 0;
}