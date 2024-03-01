/**
 * @file deadlock_case2_working.c
 * @author Benjamin Carter and Trevor Pope
 * @brief This program does not have a deadlock. The semaphore usage was adjusted and the program loops.
 * @version 0.1
 * @date 2024-02-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// Inducing a deadlock. Using two resources. They have their own mutex locks, 
// one process locks resource1 and trying to get resource2, at the same time other locks resource2 
// and trying to get resource1, so both are in deadlock.

// THIS DOES NOT HAVE A DEADLOCK

#include <pthread.h>
#include <stdio.h>
#include <time.h> 
#include <unistd.h>

pthread_mutex_t resource2Lock;
pthread_mutex_t resource1Lock;

/**
 * @brief Thread 1
 * 
 * @return void* 
 */
void *resource1()
{
    pthread_mutex_lock(&resource1Lock); // obtain resource 1
    printf("Job started in resource1..\n");
    sleep(1);
    
    printf("Trying to get resource1\n");
    pthread_mutex_unlock(&resource1Lock);

    pthread_mutex_lock(&resource2Lock);  // obtain resource 2
    printf("Aquired resource2\n");
    
    pthread_mutex_unlock(&resource2Lock);

    pthread_mutex_lock(&resource1Lock);
    printf("Job finished in resource1..\n");
    pthread_mutex_unlock(&resource1Lock);
    sleep(1);
    pthread_exit(NULL);
}

/**
 * @brief Thread 2
 * 
 * @return void* 
 */
void *resource2()
{
    pthread_mutex_lock(&resource2Lock); // obtain resource 2.
    printf("Job started in resource2..\n");
    sleep(1);
    
    printf("Trying to get resource2\n");
    pthread_mutex_unlock(&resource2Lock);

    pthread_mutex_lock(&resource1Lock);  // obtain resource 1
    printf("Aquired resource1\n");
    
    pthread_mutex_lock(&resource2Lock);

    pthread_mutex_unlock(&resource1Lock);
    printf("Job finished in resource2..\n");
    pthread_mutex_unlock(&resource2Lock);
    sleep(1);
    pthread_exit(NULL);
}

int main() 
{
  pthread_mutex_init(&resource1Lock,NULL);
  pthread_mutex_init(&resource2Lock,NULL);

  pthread_t t1,t2;

  pthread_create(&t1,NULL,resource1,NULL);
  pthread_create(&t2,NULL,resource2,NULL);

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  printf("\n");

  return main(); // repeat the program.
}
    
