#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdint.h>

typedef struct _POLINOM
{
    int grad;
    int* coefficients;
}POLINOM;

typedef struct _THREADDET
{
    int i;
    int prevResult;
    int data;
    POLINOM polinom;
}THREADDET;

void* threadFunc(void* arg)
{
    THREADDET* det = (THREADDET*)arg;
    int result;
    result = (det->prevResult * det->data + det->polinom.coefficients[det->i]);
    return (void*)result;
}

void printPolinom(POLINOM polinom)
{
    for(int i = polinom.grad;i>=0;i--)
    {
        fprintf(stdout,"%d*x^%d",polinom.coefficients[i],i);
        if(i != 0)
        {
            fprintf(stdout," + ");
        }
    }
}

int calculatePolinomA(POLINOM polinom, int value)
{
    int result = polinom.coefficients[polinom.grad];
    int currentPid = getpid();
    for(int i = polinom.grad-1;i>=0;i--)
    {
        int pid = fork();
        if(pid != currentPid)
        {
            result = result*value + polinom.coefficients[i];
            kill(pid,1);
        }
    }

    return result;
}

int calculatePolinom(POLINOM polinom, int value)
{
    int result = polinom.coefficients[polinom.grad];

    void* status = NULL;

    for(int i = polinom.grad-1;i>=0;i--)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            pthread_t tid;
            THREADDET det;
            det.i = i;
            det.prevResult = result;
            det.data = value;
            det.polinom = polinom;
            pthread_create(&tid,NULL,&threadFunc,(void*)&det);
            pthread_join(tid,&status);
            result = (int)status;
            //kill(getpid(), (int)result);
            exit((int)result);
        }
        else
        {
            waitpid(pid,&result,0);
            result = result/256;
        }
    }
    return result;
}

int main(int argc, char** argv)
{
    int n;
    int value;
    POLINOM polinom;
    fprintf(stdout,"Give n: ");
    fscanf(stdin,"%d",&n);

    polinom.grad = n;
    polinom.coefficients = (int*)malloc(n * sizeof(int));
    for(int i = 0;i<=polinom.grad;i++)
    {
        fprintf(stdout,"Give coefficient for x^%d: ",i);
        fscanf(stdin,"%d",&polinom.coefficients[i]);
    }
    printf("\n");
    printPolinom(polinom);
    fprintf(stdout,"\n\nGive value to calculate: ");
    fscanf(stdin,"%d",&value);
    fprintf(stdout,"\nCalculated only with fork: %d\n",calculatePolinomA(polinom,value));

    fprintf(stdout,"\nCalculated with pthread: %d\n",calculatePolinom(polinom,value));
    return 0;
}