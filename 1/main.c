#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_MESSAGE_SIZE 255

void functionSystemCall(char* filename)
{
    fprintf(stdout,"\n---System Calls---\n");
    int file = open(filename,O_RDONLY);
    if(file == -1)
    {
        fprintf(stdout,"Failed to open file! \n");
        return;
    }
    char* buffer = malloc(MAX_MESSAGE_SIZE);
    read(file,buffer,MAX_MESSAGE_SIZE);
    char* line;
    char delim = '\n';
    line = strtok(buffer,&delim);
    while(line != NULL)
    {
        fprintf(stdout,"%s \n\n",line);
        line = strtok(NULL,&delim);
    }
    free(buffer);
    close(file);
}

void functionStandard(char* filename)
{
    fprintf(stdout,"\n---Standard---\n");
    errno = 0;
    FILE* fin = fopen(filename,"r");
    if(fin == NULL)
    {
        fprintf(stdout,"Failed to open file! \n");
        printf("Error %d \n", errno);
    }
    char* buffer = (char*)malloc(MAX_MESSAGE_SIZE);

    while (fgets(buffer,MAX_MESSAGE_SIZE,fin) != NULL)
    {
        fprintf(stdout,"%s \n",buffer);
    }
    free(buffer);
}

int main(int argc, char *argv[])
{
    functionStandard(argv[1]);
    functionSystemCall(argv[1]);
    return 0;
}