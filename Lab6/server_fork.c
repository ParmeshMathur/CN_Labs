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

#define BUFFER_SIZE 100

int main(int argc, char const *argv[])
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock_fd<0)
	{
		printf("Socket failed\nExiting\n");
		exit(0);
	}
	else printf("Socket created\n");

	struct sockaddr_in serv_addr, cli_addr;
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

	if(listen(sock_fd, 10))
	{
		printf("Unable to listen\n");
		exit(0);
	}
	else printf("Socket listening\n");

	int len = sizeof(cli_addr);

	while(1)
	{
		int accept_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &len);
		if(accept_fd<0)
		{
			printf("Connection failed\n");
			exit(0);
		}
		else printf("Socket connected, server accepting\n");


		char buffer[BUFFER_SIZE];


		//RECEIVE STRING FROM CLIENT//
		memset(buffer, 0, sizeof(buffer));
		read(accept_fd, buffer, sizeof(buffer));
		if(strcmp(buffer, "exit")==0)
		{
			printf("Client has exited\n\n");
			close(accept_fd);
			continue;
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
	        printf("\nExiting\n");
	        close(accept_fd);
	        break;
	    }

	    //SEND MESSAGE TO CLIENT IF NOT EXIT STRING//
        n = strlen(buffer);
        write(accept_fd, buffer, n);
        // printf("Sending message to client.\n");
        printf("\n");
		close(accept_fd);
	}

	close(sock_fd);

	return 0;
}
