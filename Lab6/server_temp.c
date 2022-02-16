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
    struct sockaddr_in serv_addr, cli_addr;
    int sock_fd, accept_fd;
    int len, ret;
    pid_t childpid;
    // char clientAddr[CLADDR_LEN];

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) 
    {
        printf("Error creating socket!\n");
        exit(1);
    }
    printf("Socket created...\n");

    int port_no = atoi(argv[1]);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port_no);

    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        printf("Error binding!\n");
        exit(1);
    }
    printf("Binding done...\n");

    // printf("Waiting for a connection...\n");
    listen(sock_fd, 5);


    char buffer[BUFFER_SIZE];
    len = sizeof(cli_addr);


    while (1) 
    {
        len = sizeof(cli_addr);
        accept_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &len);
        if (accept_fd < 0) 
        {
            printf("Error accepting connection!\n");
            exit(1);
        }
        printf("Connection accepted...\n");

        // inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
        if ((childpid = fork()) == 0) 
        {
            close(sock_fd); 
            //stop listening for new connections by the main process. 
            //the child will continue to listen. 
            //the main process now handles the connected client.

            while (1) 
            {
                memset(buffer, 0, BUFFER_SIZE);
                ret = read(accept_fd, buffer, BUFFER_SIZE);
                if(ret < 0) 
                {
                    printf("Error receiving data!\n");  
                    exit(1);
                }

                int n = strlen(buffer);
                for (int i=0;i<n/2;i++)
                {
                    char temp = buffer[i];
                    buffer[i]=buffer[n-i-1];
                    buffer[n-i-1]=temp;
                }

                printf("Received data : %s\n", buffer);

                memset(buffer, 0, BUFFER_SIZE);

                fgets(buffer, BUFFER_SIZE, stdin);
                n = strlen(buffer);
                buffer[n-1] = '\0';

                ret = write(accept_fd, buffer, BUFFER_SIZE);   
                if (ret < 0) 
                {  
                    printf("Error sending data!\n");  
                    exit(1);  
                }  
                printf("Sent data : %s\n", buffer);
            }
        }
        close(accept_fd);
    }
    close(sock_fd);
}
