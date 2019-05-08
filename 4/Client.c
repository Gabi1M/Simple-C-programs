#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define MAX_MESSAGE_SIZE 50

int main()
{

    char* filename = "FIFO_file";
    char* filenameWrite = "FIFO_file_return";
    char* username = (char*)malloc(MAX_MESSAGE_SIZE);

    if(mkfifo(filename,0777) == -1)
    {
        if(errno == EEXIST)
        {
            fprintf(stdout,"Fifo %s already exists!\n",filename);
            //exit(-1);
        }
    }

    write(1,"Input the username: ",strlen("Input the username: "));
    read(0,username,50);

    int fifoDescriptor = open(filename,O_WRONLY);
    write(fifoDescriptor,username,strlen(username));
    close(fifoDescriptor);


    write(1,"Waiting for server to return...\n",strlen("Waiting for server to return...\n"));
    int fifoDescriptorRead = open(filenameWrite,O_RDONLY);
    write(1,"Server returned...\n",strlen("Server returned...\n"));

    //sleep(2);
    free(username);
    username = malloc(MAX_MESSAGE_SIZE);
    strcpy(username,"");
    read(fifoDescriptorRead,username,MAX_MESSAGE_SIZE);
    close(fifoDescriptorRead);

    printf("%s\n",username);

    return 0;
}