#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <pthread.h>
 
#define BUFFER_SIZE 4096
 
extern int errno;
 
char* priv_key_file_name;
char* pub_key_file_name;
 
int sock_fd;
 
void* decryptor (void* param)
{
	char ciphertext[BUFFER_SIZE];
	char plaintext[BUFFER_SIZE];
 
	FILE* priv_key_fp =fopen(priv_key_file_name,"r");
    if(priv_key_fp==NULL)
    {
        printf("Private key file not accessibe\n");
        exit(1);
    }
 
    
    RSA* rsa_priv = PEM_read_RSAPrivateKey(priv_key_fp, NULL, NULL, NULL);
    if(rsa_priv==NULL)
    {
        printf("Couldn't read private key\n");
        exit(1); 
    }
    int encrypt_len = RSA_size(rsa_priv);
 
	while(1)
	{
		//RECEIVE STRING FROM SERVER//
		memset(plaintext, 0, sizeof(plaintext));
        memset(ciphertext, 0, sizeof(ciphertext));
 
        if(read(sock_fd, ciphertext, sizeof(ciphertext))<0)
        {
            printf("Didn't read\n");
            perror(strerror(errno));
            close(sock_fd);
            exit(1);
        }
        else printf("length %ld\n", strlen(ciphertext));

        if(strcmp(ciphertext, "exit")==0)	
        {
            printf("Exiting\n");
            close(sock_fd);
            exit(0);
        }
        // int encrypt_len = strlen(ciphertext);
 
        //DECRYPT STRING//
        int decrypt_len = RSA_private_decrypt(encrypt_len, ciphertext, plaintext, rsa_priv, RSA_PKCS1_PADDING);
	    // if(decrypt_len==-1)
	    // {
	    //     printf("Decryption failed\n");
	    //     perror(strerror(errno));
	    //     close(sock_fd);
	    //     exit(1);
	    // }
 
        //CHECK EXIT CONDITION//
        if(strcmp(plaintext, "exit")==0)
        {
            printf("Exited\n");
            close(sock_fd);
            exit(0);
        }
        // else printf("Buffer read\n");
 
        if(strlen(plaintext)>=0) 
    	{
    		printf("ciphertext: %s\n", ciphertext);
    		printf("plaintext: %s\n\n", plaintext);
    	}
	}
    pthread_exit(0);
}
 
void* encryptor (void* param)
{
	char plaintext[BUFFER_SIZE];
	char ciphertext[BUFFER_SIZE];
 
	FILE* pub_key_fp=fopen(pub_key_file_name,"r");
    if(pub_key_fp==NULL)
    {
        printf("Public key file not accessibe\n");
        exit(1);
    }
 
    RSA* rsa_pub = PEM_read_RSA_PUBKEY(pub_key_fp, NULL, NULL, NULL);
    if(rsa_pub==NULL)
    {
        printf("Couldn't read public key\n");
        exit(1);
    }
 
    while(1)
    {
        //TAKE INPUT STRING//
        memset(plaintext, 0, sizeof(plaintext));
        memset(ciphertext, 0, sizeof(ciphertext));
        read(0, plaintext, BUFFER_SIZE);
 
        int n = strlen(plaintext);
        plaintext[n-1]='\0';
        if(strcmp(plaintext,"exit")==0)
        {
            // exit_flag=1;
            write(sock_fd, "exit", sizeof("exit"));
        	printf("Exiting\n");
        	close(sock_fd);
        	exit(0);
        }
        n=strlen(plaintext);
 
        //ENCRYPT STRING//
        int encrypt_len=RSA_public_encrypt(n/*strlen(plaintext)*/, plaintext, ciphertext, rsa_pub, RSA_PKCS1_PADDING);
	    if(encrypt_len==-1 )
	    {
	        printf("Encryption failed\n");
	        close(sock_fd);
	        exit(1);
	    }
	    else
	    {
	    	printf("length %ld\n", strlen(ciphertext));
	    	printf("ciphertext: %s\n", ciphertext);
	    }
 
        //SEND ENCRYPTED MESSAGE TO SERVER//
        n = strlen(ciphertext);
        
        // if(write(sock_fd, ciphertext, BUFFER_SIZE)<0) 
        if(write(sock_fd, ciphertext, n)<0)
        {
            printf("Server is unreachable\n");
            close(sock_fd);
            exit(1);
        }
        // else printf("Buffer written\n");
        else printf("Sent\n\n");
    }
    pthread_exit(0);
}
 
int main(int argc, char const *argv[])
{
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd<0)
    {
        printf("Socket not created\nExiting\n");
        exit(1);
    }
    else printf("Socket created\n");
 
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	int port_no = atoi(argv[2]);
 
	serv_addr.sin_family = AF_INET;
	inet_aton(argv[1], &serv_addr.sin_addr);
	serv_addr.sin_port = htons(port_no);
 
	if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) 
	{ 
        printf("Server is unreachable\nExiting\n"); 
        exit(1);
    }
    else printf("Conected to server\n");
 
    pthread_t tid_encrypt;
    pthread_t tid_decrypt;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	// pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
 
	priv_key_file_name = (char*)malloc(strlen(argv[3]));
	strcpy(priv_key_file_name, argv[3]);
	pub_key_file_name = (char*)malloc(strlen(argv[4]));
	strcpy(pub_key_file_name, argv[4]);
    printf("last param %s\n", pub_key_file_name);
 
	pthread_create(&tid_decrypt, &attr, decryptor, NULL);
	pthread_create(&tid_encrypt, &attr, encryptor, NULL);
	
	pthread_join(tid_decrypt, NULL);
	pthread_join(tid_encrypt, NULL);
 
	return 0;
}