/*
 * main.c
 *
 *  Created on: 2011. 12. 28.
 *      Author: marc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <sys/time.h>

#define NUMBER_OF_POINT 300
#define NUMBER_OF_LINE 400
#define NUMBER_OF_SAMPLE 100

typedef struct
{
    int size;
    unsigned char connection[NUMBER_OF_POINT];
    unsigned char shape[NUMBER_OF_POINT];
} _point;

int findMaxSize(_point *mypoint, int numberOfPoint)
{
    int returnValue = 0;
    int i;

    for(i = 0; i < numberOfPoint; i++)
    {
        if(mypoint[i].size > returnValue)
        {
            returnValue = mypoint[i].size;
        }
    }

    return returnValue;
}

int connectPoint(int numberOfPoint, int numberOfLine)
{
    int select1, select2;
    int i, j;
    int maxSize;
    _point *mypoint;
    struct timeval val;

    // mypoint ���� �� �ʱ�ȭ (size = 1, �ڽŰ��� connection = 1, �ڽŰ��� shape = 1)
    mypoint = (_point *)malloc(sizeof(_point)*numberOfPoint);

    for(i = 0; i < numberOfPoint; i++)
    {
        mypoint[i].size = 1;

        for(j = 0; j < numberOfPoint; j++)
        {
            if(i == j)
            {
                mypoint[i].connection[j] = 1;
                mypoint[i].shape[j] = 1;
            }
            else
            {
                mypoint[i].connection[j] = 0;
                mypoint[i].shape[j] = 0;
            }
        }
    }

    // �������� ���� ������ �� point ����
    gettimeofday(&val, NULL);
    srand((unsigned)val.tv_usec);

    for(i = 0; i < numberOfLine; i++)
    {
        do
        {
            select1 = rand()%numberOfPoint;
            select2 = rand()%numberOfPoint;
        } while(select1 == select2);

        // ������ �� ����Ʈ�� ���� ����Ǵ� ��� size ���� �� connection ���� ������Ʈ
        // ����Ǿ� �ִ� ��� point�� size �� connection ������Ʈ
        // ����� �� point�� shape ������Ʈ
        if(mypoint[select1].connection[select2] != 1)
        {
            // �� point�� ����� ���Ͽ� ù��° point�� �ι�° point�� ������Ʈ
            mypoint[select1].size += mypoint[select2].size;

            // �� point�� shape�� 1�� ����
            mypoint[select2].shape[select1] = 1;
            mypoint[select1].shape[select2] = 1;

            for(j = 0; j < numberOfPoint; j++)
            {
                if(mypoint[select2].connection[j] == 1)
                {
                    mypoint[select1].connection[j] = 1;
                }
            }


            for(j = 0; j < numberOfPoint; j++)
            {
                if((mypoint[select1].connection[j] == 1) && (select1 != j))
                {
                    mypoint[j].size = mypoint[select1].size;
                    memcpy(&mypoint[j].connection[0], &mypoint[select1].connection[0], numberOfPoint);
                }
            }
        }
    }

    maxSize = findMaxSize(mypoint, numberOfPoint);

    //printf("\nmax size = %d", maxSize);

    free(mypoint);

    return maxSize;
}

int main()
{
    int numberOfPoint = NUMBER_OF_POINT;
    int numberOfLine;
    int sumValue = 0;
    float meanValue = 0.0;
    int i, j;

    for(i = 5; i <= NUMBER_OF_LINE; i += 5)
    {
        numberOfLine = i;
        sumValue = 0;

        for(j = 0; j < NUMBER_OF_SAMPLE; j++)
        {
            sumValue += connectPoint(numberOfPoint, numberOfLine);
            usleep(1000);
        }

        meanValue = (float)sumValue / (float)j;

        //printf("\nnumber of line = [%d], meanValue = [%.2f]", numberOfLine, meanValue);
        printf("\n%d, %.2f", numberOfLine, meanValue);
    }

    return 1;
}
