

#include <stdio.h>
#include <stdbool.h>

#define SIZE  3

int buffer[SIZE];
int start;

int length = 0;

bool isFull()
{
     return length == 0;
}

void put(int a)
{
    int endTemp = (start + length) % SIZE;
    if(endTemp == start && length != 0)
    {
        // queue full!
        return;
    }
    length++;
    buffer[endTemp] = a;
}

int get()
{
    if(length == 0)
    {
        // nothing in the queue!
        return 0;
    }
    int startTemp = start;
    start = (start + 1) % SIZE;
    length--;
    return buffer[startTemp];
}


int main()
{
    put(4);
    put(2);
    put(1);
    put(9);
    while(length != 0)
   {
       int c = get();
       printf("%d\n",c);
   }
    return 0;
}
