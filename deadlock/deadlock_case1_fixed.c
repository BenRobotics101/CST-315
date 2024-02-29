// Inducing a deadlock. Using two resources. They have their own mutex locks, 
// one process locks resource1 and trying to get resource2, at the same time other locks resource2 
// and trying to get resource1, so both are in deadlock.

// CASE 1

#include <pthread.h>
#include <stdio.h>
#include <time.h> 
#include <unistd.h>


pthread_mutex_t lock1;
pthread_mutex_t lock2;

pthread_mutex_t resource2Lock;
pthread_mutex_t resource1Lock;

void *resource1()
{
    pthread_mutex_lock(&resource1Lock);
    printf("Job started in resource1..\n");
    sleep(1);
    
    printf("Trying to get resource1\n");
    pthread_mutex_unlock(&resource1Lock);

    pthread_mutex_lock(&resource2Lock); 
    printf("Aquired resource2\n");
    
    pthread_mutex_unlock(&resource2Lock);

    pthread_mutex_lock(&resource1Lock);
    printf("Job finished in resource1..\n");
    pthread_mutex_unlock(&resource1Lock);

    pthread_exit(NULL);
}

void *resource2()
{
    pthread_mutex_lock(&resource2Lock);
    printf("Job started in resource2..\n");
    sleep(1);
    
    printf("Trying to get resource2\n");
    pthread_mutex_unlock(&resource2Lock);

    pthread_mutex_lock(&resource1Lock); 
    printf("Aquired resource1\n");
    
    pthread_mutex_lock(&resource2Lock);

    pthread_mutex_unlock(&resource1Lock);
    printf("Job finished in resource2..\n");
    pthread_mutex_unlock(&resource2Lock);
    
    pthread_exit(NULL);
}

int main() 
{
  pthread_mutex_init(&lock1,NULL);
  pthread_mutex_init(&lock2,NULL);

  pthread_t t1,t2;

  pthread_create(&t1,NULL,resource1,NULL);
  pthread_create(&t2,NULL,resource2,NULL);

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);

  return 0;
}
    
