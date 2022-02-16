#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

int fd1, fd2; 

void* to_server()
{
	char arr[80];
	fd1 = open("c_s", O_WRONLY);
	while(1)
	{
        fgets(arr, 80, stdin);
        // printf("%s\n", arr);

        if(strcmp(arr, "exit\n")==0)
        {
        	printf("exiting\n");
        	close(fd1);
        	close(fd2);
        	unlink("c_s");
        	unlink("s_c");
            
        	exit(0);
        } 

        if(write(fd1, arr, strlen(arr)+1)==-1) printf("\nError\n");
    }
    close(fd1);
    pthread_exit(0);
}



void* from_server()
{
	char array[80];
	fd2 = open("s_c", O_RDONLY);
	while(1)
	{
		if(read(fd2, array, sizeof(array)))
        {
            printf("\nServer: %s\n", array);
        }
	}
	close(fd2);
	pthread_exit(0);
}



int main(int argc, char const *argv[])
{
    pthread_t tids_c, tidc_s;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tidc_s, &attr, to_server, NULL);
    pthread_create(&tids_c, &attr, from_server, NULL);

    pthread_join(tidc_s, NULL);
    pthread_join(tids_c, NULL);

    return 0;
}