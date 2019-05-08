#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <semaphore.h>

#define MAX_MESSAGE_SIZE 50

int main()
{
    char* filenameRead = "FIFO_file";
    char* filenameWrite = "FIFO_file_return";
    char* username = (char*)malloc(MAX_MESSAGE_SIZE);

    if(mkfifo(filenameWrite,0777) == -1)
    {
        if(errno == 17)
        {
            fprintf(stdout,"Fifo %s already exists!\n",filenameWrite);
            //exit(-1);
        }
    }


    int fifoDescriptorRead = open(filenameRead,O_RDONLY);
    //sleep(2);
    read(fifoDescriptorRead,username,MAX_MESSAGE_SIZE);
    close(fifoDescriptorRead);

    char* script = (char*)malloc(MAX_MESSAGE_SIZE*2);
    strcpy(script,"/home/gabriel/scriptusers.sh ");
    char delim = '\n';
    strtok(username,&delim);
    strcat(script,username);

    int res = system(script) /256;
    free(script);

    if(res > 1000)
    {
        strcpy(username,"ERROR");
    }
    else
    {
        sprintf(username,"%d",res);
    }

    int fifoDescriptorWrite = open(filenameWrite,O_WRONLY);
    write(fifoDescriptorWrite,username,strlen(username));
    close(fifoDescriptorWrite);

    return 0;
}