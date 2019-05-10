#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <stdlib.h>
#include <wait.h>

int readNumber(const int file)
{
    char c;

    if(read(file,&c,1) == 0)
    {
        printf("Failed to read from file!\n");
        return -1;
    }
    lseek(file,1,SEEK_CUR);
    return atoi(&c);
}

int readNumberV2(const int file)
{
    char c;
    int number = 0;

    do
    {
        read(file,&c,1);
        if(c >= '0' && c <= '9')
        {
            int digit = atoi(&c);
            number *= 10;
            number += digit;
        }
    }while(c != ' ');

    return number;
}

void readArray(int** array, const int file, const int count)
{
    for(int i = 0;i<count;i++)
    {
        (*array)[i] = readNumberV2(file);
    }
}

void printArray(const int* array, const int count)
{
    for(int i = 0;i<count;i++)
    {
        printf("%d ",array[i]);
    }
    printf("\n");
}

int getMax(const int* array, const int count)
{
    int max = array[0];
    for(int i = 1;i<count;i++)
    {
        if(array[i] > max)
        {
            max = array[i];
        }
    }

    return max;
}

int getMin(const int* array, const int count)
{
    int min = array[0];
    for(int i = 1;i<count;i++)
    {
        if(array[i] < min)
        {
            min = array[i];
        }
    }

    return min;
}

int removeNumber(int** array, int* count, int number)
{
    for(int i = 0;i<(*count);i++)
    {
        if((*array)[i] == number)
        {
            for(int j = i;j<(*count)-1;j++)
            {
                (*array)[j] = (*array)[j+1];
            }
            (*count)--;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    int file = open(argv[1],O_RDONLY);
    if(file == -1)
    {
        printf("Failed to open file!\n");
        return -1;
    }

    int n = readNumberV2(file);

    int* numbers = (int*)calloc(n,sizeof(int));
    readArray(&numbers,file,n);

    close(file);

    __pid_t idMin = -1;
    __pid_t idMax = -1;

    int resultMin;
    int resultMax;

    int k = atoi(argv[2]);

    printf("Numbers read from file: ");
    printArray(numbers,n);

    while(1)
    {
        idMin = fork();

        if(idMin != 0)
        {
            idMax = fork();
        }

        if(idMin == 0)
        {
            exit(getMin(numbers,n));
        }

        if(idMax == 0)
        {
            exit(getMax(numbers,n));
        }

        waitpid(idMin,&resultMin,0);
        waitpid(idMax,&resultMax,0);

        removeNumber(&numbers,&n,resultMin/256);
        k--;
        if(k == 0)
        {
            break;
        }

        removeNumber(&numbers,&n,resultMax/256);
        k--;
        if(k == 0)
        {
            break;
        }
    }

    printf("Numbers left after processing: ");
    printArray(numbers,n);
    printf("The %d-th element: %d\n",atoi(argv[2]),getMax(numbers,n));

    free(numbers);

    return 0;
}