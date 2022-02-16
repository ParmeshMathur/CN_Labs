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
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd<0)
    {
        printf("Socket not created\nExiting\n");
        exit(0);
    }

	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	int port_no = atoi(argv[2]);

	serv_addr.sin_family = AF_INET;
	inet_aton(argv[1], &serv_addr.sin_addr);
	serv_addr.sin_port = htons(port_no);

	if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) 
	{ 
        printf("Server is unreachable\nExiting\n"); 
        exit(0);
    }

    char buffer[20];
    
    while(1)
    {
        printf("Enter required file name: ");
        scanf("%s", buffer);
        if(strcmp(buffer,"exit")==0)
        {
            printf("\nExiting\n");
            close(sock_fd);
            exit(0);
        }
        char filename[20];
        strcpy(filename, buffer);
        // printf("Not written yet\n");
        if(write(sock_fd, buffer, sizeof(buffer))<0) 
        {
            printf("Server is unreachable\n");
            exit(0);
        }
        else printf("Buffer written\n");

        memset(buffer, 0, 20*sizeof(buffer[0]));
        // printf("memset\n");

        if(read(sock_fd, buffer, sizeof(buffer))<0)
        {
            printf("didn't read\n");
            exit(0);
        }
        else printf("Buffer Read\n");

        int file_fd = open(filename, O_WRONLY | O_CREAT, 00777);
        // printf("File created\n");
        int i=0;
        while(buffer[i]!=0) i++;
        // printf("%s\n", buffer);
        write(file_fd, buffer, i*sizeof(char));
        printf("\nFile written.\n\n");
        close(file_fd);
    }

    close(sock_fd);

	return 0;
}
