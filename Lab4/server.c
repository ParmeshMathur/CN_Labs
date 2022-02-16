#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

int fd1, fd2; //declare golbal file decriptors, to close when exiting

void* to_client()
{
    char arr[80];
    fd1 = open("s_c", O_WRONLY);
    while(1)
    {
        fgets(arr, 80, stdin); 
        // printf("%s\n", arr);

        if(strcmp(arr, "exit\n")==0) //If string input is "exit", exit from process
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



void* from_client()
{
    char array[80];
    fd2 = open("c_s", O_RDONLY);
    while(1)
    {
        if(read(fd2, array, sizeof(array))) //Check if pipe is empty, if not print
        {
            printf("\nClient: %s\n", array);
        }
    }
    close(fd2);
    pthread_exit(0);
}



int main(int argc, char const *argv[])
{
    mkfifo("s_c", 0666); //For server to client communication
    mkfifo("c_s", 0666); //For client to server communication

    //Initializing thread attributes
    pthread_t tids_c, tidc_s;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    //Creating threads
    pthread_create(&tidc_s, &attr, from_client, NULL);
    pthread_create(&tids_c, &attr, to_client, NULL);

    //Joining threads
    pthread_join(tidc_s, NULL);
    pthread_join(tids_c, NULL);

    return 0;
}