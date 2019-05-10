#include <stdio.h>
#include <signal.h>
#include <zconf.h>

void add(int* a, const int* b)
{
    printf("%d + %d = ",*a,*b);
    (*a) += (*b);
    printf("%d\n",*a);
}

void subtract(int* a, const int* b)
{
    printf("%d - %d = ",*a,*b);
    (*a) -= (*b);
    printf("%d\n",*a);
}

void multiply(int* a, const int* b)
{
    printf("%d * %d = ",*a,*b);
    (*a) *= (*b);
    printf("%d\n",*a);
}

void divide(int* a, const int* b)
{
    printf("%d / %d = ",*a,*b);
    (*a) /= (*b);
    printf("%d\n",*a);
}

void print(const int* a, const int* b)
{
    printf("a = %d,  b = %d\n",(*a),(*b));
}

void readNum(int* a, int* b)
{
    printf("Give initial values for a and b\n");
    scanf("%d %d",a,b);
}

void handlerFPE(int signal)
{
    if(signal == SIGFPE)
    {
        printf("Received SIGFPE!\n"
               "Raising SIGINT!\n");
        raise(SIGINT);
    }
}

void handlerINT(int signal)
{
    if(signal == SIGINT)
    {
        printf("Received SIGINT!\n"
               "Closing...\n");
        _exit(SIGINT);
    }
}

void handlerHUP(int signal)
{
    if(signal == SIGHUP)
    {
        printf("Received SIGHUP!\n"
               "Waiting...\n");

        sleep(10);

        printf("Resuming...\n");
    }
}

int main()
{
    void (*ptrArr[])(int*,const int*) = {&add,&subtract,&multiply,&divide};
    void (*ptrPrint)(const int*, const int*) = &print;
    void (*ptrRead)(int*, int*) = &readNum;
    int a = 0;
    int b = 0;

    signal(SIGFPE,handlerFPE);
    signal(SIGINT,handlerINT);
    signal(SIGHUP,handlerHUP);

    (*ptrRead)(&a,&b);

    printf("0. Exit\n"
           "1. Add a to b\n"
           "2. Add b to a\n"
           "3. Subtract b from a\n"
           "4. Subtract a from b\n"
           "5. Multiply a with b\n"
           "6. Multiply b with a\n"
           "7. Divide a with b\n"
           "8. Divide b with a\n"
           "9. Wait 3 seconds\n");

    int choice = 0;
    while(1)
    {
        enter:

        scanf("%d",&choice);

        switch (choice)
        {
            case 0:
            {
                goto end;
            }

            default:
            {
                printf("Unknown!\n");
            }

            case 1:
            {
                (*ptrArr)(&a,&b);
                (*ptrPrint)(&a,&b);
                break;
            }

            case 2:
            {
                (*ptrArr)(&b,&a);
                (*ptrPrint)(&a,&b);
                break;
            }

            case 3:
            {
                (*(ptrArr + 1))(&a,&b);
                (*ptrPrint)(&a,&b);
                break;
            }

            case 4:
            {
                (*(ptrArr + 1))(&b,&a);
                (*ptrPrint)(&a,&b);
                break;
            }

            case 5:
            {
                (*(ptrArr + 2))(&a,&b);
                (*ptrPrint)(&a,&b);
                break;
            }

            case 6:
            {
                (*(ptrArr + 2))(&b,&a);
                (*ptrPrint)(&a,&b);
                break;
            }

            case 7:
            {
                if((b) == 0)
                {
                    printf("!!!!Division by zero detected!!!!\n");
                    break;
                }
                (*(ptrArr + 3))(&a,&b);
                (*ptrPrint)(&a,&b);
                break;
            }

            case 8:
            {
                (*(ptrArr + 3))(&b,&a);
                (*ptrPrint)(&a,&b);
                break;
            }

            case 9:
            {
                raise(SIGHUP);
                break;
            }
        }
    }
    end:

    raise(SIGINT);
}