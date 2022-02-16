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

#define BUFFER_SIZE 200

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
    else printf("Connected to server\n");


    char buffer[BUFFER_SIZE];
    
    while(1)
    {
        //TAKE INPUT STRING//
        memset(buffer, 0, sizeof(buffer));

        printf("Enter string: ");
        scanf("%[^\n]s", buffer);
        int n = strlen(buffer);
        if(strcmp(buffer,"exit")==0)
        {
            write(sock_fd, buffer, n);
            printf("\nExiting\n");
            break;
            // close(sock_fd);
            // exit(0);
        }


        //SEND MESSAGE TO SERVER IF NOT EXIT STRING//
        if(write(sock_fd, buffer, n)<0) 
        {
            printf("Server is unreachable\n");
            close(sock_fd);
            exit(0);
        }
        // else printf("Buffer written\n");


        memset(buffer, 0, sizeof(buffer));
        // printf("memset\n");


        //RECEIVE STRING FROM SERVER//
        if(read(sock_fd, buffer, sizeof(buffer))<0)
        {
            printf("Didn't read\n");
            close(sock_fd);
            exit(0);
        }
        // else printf("Buffer read\n");


        //REVERSE STRING//
        n = strlen(buffer);
        for (int i=0;i<n/2;i++)
        {
            char temp = buffer[i];
            buffer[i]=buffer[n-i-1];
            buffer[n-i-1]=temp;
        }
        printf("%s\n", buffer);
        printf("\n");
        memset(buffer, 0, sizeof(buffer));
    }

    close(sock_fd);

	return 0;
}
