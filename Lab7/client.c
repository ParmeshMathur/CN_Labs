#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>

#define BUFFER_SIZE 1024

int getContentLength(char* buffer)
{
    char* p;
    p=strstr(buffer, "Content-Length:");
    p+=16;

    char len[7];
    bzero(len, 7);
    int j=0;
    while(*p>='0' && *p<='9')
    {
        len[j]=*p;
        p+=1;
        j++;
    }
    len[j]='\0';

    printf("%s\n", len);

    int content_length = atoi(len);

    return content_length;
}

FILE* fileCreate(char* filename)
{
    FILE* fptr;
    fptr = fopen(filename, "wb");
    if(fptr==NULL)
    {
        printf("FIle not opened\n");
        exit(1);
    }
    printf("File opened\n");
    return fptr;
}

int main(int argc, char const *argv[])
{
    if(argc!=2)
    {
        printf("Usage: %s <URL>\n", argv[0]);
        exit(1);
    }
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    //TOKENIZING THE URL//
    // char protocol[6];
    char hostname[50];
    char destname[50];
    char filename[50];
    // memset(protocol, 0, 6);
    memset(hostname, 0, 50);
    memset(destname, 0, 50);
    memset(filename, 0, 50);


    char* ptr;
    ptr=strstr(argv[1], "://");
    ptr+=3;
    printf("%s\n", ptr);

    int i=0;

    while(*ptr != '/')
    {
        hostname[i]=*ptr;
        ptr+=1;
        i++;
    }
    hostname[i]='\0';
    printf("hostname=%s\n", hostname);
    i=0;
    strcpy(destname, ptr);
    ptr=strrchr(argv[1], '/');
    ptr+=1;
    strcpy(filename, ptr);
    // while(argv[1][i]!='/')
    // {
    //     protocol[i]=argv[1][i];
    //     i++;
    // }
    // i+=2;
    // while(argv[1][i]!='/')
    // {
    //     hostname[j]=argv[1][i];
    //     i++;
    //     j++;
    // }
    // j=0;
    // while(i<strlen(argv[1]))
    // {
    //     destname[j]=argv[1][i];
    //     j++;
    //     i++;
    // }
    // int k=strlen(argv[1])-1;
    // while(argv[1][k]!='/') k--;
    // k++;
    // j=0;
    // while(k<strlen(argv[1]))
    // {
    //     filename[j]=argv[1][k];
    //     k++;
    //     j++;
    // }
    //DONE

    //PREPARING REQUEST//
    char header[100] = "GET ";
    char headmid[20] = " HTTP/1.1\r\nHost: ";
    char headend[30] = "\r\nConnection: close\r\n\r\n";
    strcat(header, destname);
    strcat(header, headmid);
    strcat(header, hostname);
    strcat(header, headend);

    //DONE

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    struct hostent *hp;

    if((hp = gethostbyname(hostname)) == NULL)
    {
        herror("gethostbyname");
        exit(1);
    }

    if(argv[1][4]==':')
    {
        bcopy(hp->h_addr, &serv_addr.sin_addr, hp->h_length);
        serv_addr.sin_port = htons(80);
        serv_addr.sin_family = AF_INET;

        if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) 
        { 
            printf("Server is unreachable\nExiting\n"); 
            exit(1);
        }

        //SENDING REQUEST//
        printf("%s\n", header);

        if(write(sock_fd, header, strlen(header))<0)
        {
            printf("Server is unreachable\n");
            exit(1);
        }
        printf("Request sent\n");
        //DONE


        //CREATE/OVERWRITE FILE//
        FILE* fptr = fileCreate(filename);
        //DONE//


        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);

        // read(sock_fd, buffer, BUFFER_SIZE);
        // write(file_fd, buffer, strlen(buffer));

        //FIND LENGTH OF CONTENT//
        // int content_length = getContentLength(buffer);
        // printf("Content length: %d\n", content_length);
        // printf("%s", buffer);
        //DONE//

        // int bytes_recvd=0;
        // bytes_recvd+=strlen(buffer);
        // printf("Length read: %d\n", bytes_recvd);
        bzero(buffer, BUFFER_SIZE);
        int temp;

        while((temp = read(sock_fd, buffer, BUFFER_SIZE))!=0)
        {
            ptr=strstr(buffer, "\r\n\r\n");
            
            printf("temp=%d\n", temp);
            // printf("%lu\n", strlen(buffer));
            if(ptr==NULL) fwrite(buffer, temp, 1, fptr);
            else
            {
                char* rcode;
                rcode = strstr(buffer, "HTTP/1.1 ");
                rcode+=9;
                int x = 0, j=0;
                while(j<3)
                {
                    x*=10;
                    x+=(*rcode)-'0';
                    j++;
                    rcode+=1;
                }
                if(x!=200)
                {
                    printf("File not reachabele, response code: %d\n", x);
                    exit(1);
                }
                ptr+=4;
                temp-=(ptr-buffer);
                fwrite(ptr, temp, 1, fptr);
            }
            // bytes_recvd+=temp;
            bzero(buffer, BUFFER_SIZE);
            // printf("Length read: %d\n", bytes_recvd);
            // if(bytes_recvd>=content_length) break;
        }
        printf("\nFile written\n");

        
        //CLOSE OPEN DECRIPTORS//
        // shutdown(sock_fd, SHUT_RDWR); 
        fclose(fptr);
        close(sock_fd);
    }

    else
    {
        bcopy(hp->h_addr, &serv_addr.sin_addr, hp->h_length);
        serv_addr.sin_port = htons(443);
        serv_addr.sin_family = AF_INET;

        if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) 
        { 
            printf("Server did not\nExiting\n"); 
            exit(1);
        }

        //SSL REQUIREMENTS
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
        SSL_library_init();
        SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());

        SSL *conn = SSL_new(ssl_ctx);
        SSL_set_fd(conn, sock_fd);

        if (SSL_connect(conn) != 1)
        {
            printf("Server is unreachable\nExiting\n"); 
            exit(1);
        }
        //DONE


        //SENDING REQUEST//
        printf("%s\n", header);

        if(SSL_write(conn, header, strlen(header))<0)
        {
            printf("Server is unreachable\n");
            exit(1);
        }
        printf("Request sent\n");
        //DONE//


        //CREATE/OVERWRITE A FILE//
        FILE* fptr = fileCreate(filename);
        //DONE//


        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);

        // int temp = SSL_read(conn, buffer, BUFFER_SIZE);
        // fwrite(fptr, buffer, strlen(buffer));

        //FIND LENGTH OF CONTENT//
        // int content_length = getContentLength(buffer);
        // printf("Content length: %d\n", content_length);
        // printf("%s", buffer);
        //DONE//


        // int bytes_recvd=0;
        // bytes_recvd+=strlen(buffer);
        // printf("Length read: %d\n", bytes_recvd);
        bzero(buffer, BUFFER_SIZE);
        // int c=0;
        int temp=0;

        while((temp = SSL_read(conn, buffer, BUFFER_SIZE))!=0)
        {
            ptr=strstr(buffer, "\r\n\r\n");
            
            printf("temp=%d\n", temp);
            // printf("%lu\n", strlen(buffer));
            if(ptr==NULL) fwrite(buffer, temp, 1, fptr);
            else
            {
                char* rcode;
                rcode = strstr(buffer, "HTTP/1.1 ");
                rcode+=9;
                int x = 0, j=0;
                while(j<3)
                {
                    x*=10;
                    x+=(*rcode)-'0';
                    j++;
                    rcode+=1;
                }
                if(x!=200)
                {
                    printf("File not reachabele, response code: %d\n", x);
                    exit(1);
                }
                ptr+=4;
                temp-=(ptr-buffer);
                fwrite(ptr, temp, 1, fptr);
            }
            // bytes_recvd+=temp;
            bzero(buffer, BUFFER_SIZE);
            // printf("Length read: %d\n", bytes_recvd);
            // if(bytes_recvd>=content_length) break;
        }
        printf("\nFile written\n");

        //CLOSE OPEN DESCRIPTORS AND STRUCTURES//
        // shutdown(sock_fd, SHUT_RDWR); 
        fclose(fptr);
        SSL_free(conn);
        close(sock_fd);
        SSL_CTX_free(ssl_ctx);
    }

    return 0;
}