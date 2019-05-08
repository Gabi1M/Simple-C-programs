#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 30

int anonymous_pipe[2];
pid_t pids[3];

int main()
{
    char* lineRead = (char*)malloc(MAX_MESSAGE_SIZE);
    write(1,"Enter string:",strlen("Enter string:"));
    read(0, lineRead, MAX_MESSAGE_SIZE);
    int len = 0;
    while(lineRead[len] != '\n')
    {
        len++;
    }
    pids[0] = getpid();

    if(pipe(anonymous_pipe) < 0)
    {
        write(0,"Failed to create anonymous pipe\n",sizeof("Failed to create anonymous pipe\n"));
        return -1;
    }

    write(anonymous_pipe[1],lineRead,len);

    if(fork() == 0)
    {

        write(1,"In process 1\n\n", strlen("In process 1\n\n"));

        pids[1] = getpid();
        char* input1 = (char*)malloc(len);

        read(anonymous_pipe[0],input1,len);
        write(1,"Found numbers: ",strlen("Found numbers: "));

        for(int i = 0;i<len;i++)
        {
            if(input1[i] >= '0' && input1[i] <= '9')
            {
                write(1,&input1[i],1);
                write(1," ",1);
            }
        }
        write(1,"\n\n",2);

        free(input1);

        write(1,"Process 1 finished\n\n",strlen("Process 1 finished\n\n"));

        exit(0);
    }

    waitpid(pids[1],NULL,0);

    write(anonymous_pipe[1],lineRead,len);
    close(anonymous_pipe[1]);

    if(fork() == 0)
    {
        write(1,"In process 2\n\n",strlen("In process 2\n\n"));

        pids[2] = getpid();
        char* input2 = (char*)malloc(len);

        read(anonymous_pipe[0],input2,len);
        write(1,"Transformed sentence: ",strlen("Transformed sentence: "));
        for(int i = 0;i<len;i++)
        {
            if(input2[i] >= 'a' && input2[i] <= 'z')
            {
                input2[i] -= 32;
                write(1,&input2[i],1);
                write(1," ",1);
            }
        }
        //write(1,input2,len);
        write(1,"\n\n",2);
        free(input2);
        write(1,"Process 2 finished\n\n",strlen("Process 2 finished\n\n"));
        exit(0);
    }

    waitpid(pids[2],NULL,0);

    free(lineRead);
    close(anonymous_pipe[1]);

    return 0;
}