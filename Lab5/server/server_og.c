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

int main(int argc, char const *argv[])
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serv_addr, cli_addr;

	memset(&serv_addr, 0, sizeof(struct sockaddr_in));

	int port_no = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	// inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port_no);

	bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(sock_fd, 1);

	int len = sizeof(cli_addr);
	int conn_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &len);

	while(1)
	{
		char buffer[20];
		memset(buffer, 0, 20*sizeof(buffer[0]));
		read(conn_fd, buffer, sizeof(buffer));
		int file_fd;
		if(access(buffer, F_OK)==0)
		{
			file_fd = open(buffer, O_RDONLY);
			memset(buffer, 0, 20*sizeof(buffer[0]));
			read(file_fd, buffer, 10);
			write(conn_fd, buffer, 10);
			printf("Sending message to client.\n");
		}
		else 
		{
			memset(buffer, 0, 20*sizeof(buffer[0]));
			buffer[0] = '\0';
			write(conn_fd, buffer, 10);
			printf("Sending empty message to client.\n");
		}
		// printf("Sending message to client.\n");
		close(file_fd);
	}

	close(sock_fd);
	close(conn_fd);

	return 0;
}