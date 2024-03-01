/**
 * @file deadlock_case1.c
 * @author Benjamin Carter and Trevor Pope
 * @brief This program does have a deadlock.
 * @version 0.1
 * @date 2024-02-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */
// Inducing a deadlock. Using two resources. They have their own mutex locks, 
// one process locks resource1 and trying to get resource2, at the same time other locks resource2 
// and trying to get resource1, so both are in deadlock.

// THIS HAS A DEADLOCK.

#include <pthread.h>
#include <stdio.h>
#include <time.h> 
#include <unistd.h>


pthread_mutex_t lock1;
pthread_mutex_t lock2;

/**
 * @brief Thread 1
 * 
 * @return void* 
 */
void *resource1()
{

  pthread_mutex_lock(&lock1);

  printf("Job started in resource1..\n");
  sleep(2);

  printf("Trying to get resoure2\n");
  pthread_mutex_lock(&lock2); 
  printf("Aquired resourc2\n");
  pthread_mutex_unlock(&lock2);

  printf("Job finished in resource1..\n");

  pthread_mutex_unlock(&lock1);

  pthread_exit(NULL);
}

/**
 * @brief Thread 2
 * 
 * @return void* 
 */
void *resource2()
{
  pthread_mutex_lock(&lock2);

  printf("Job started in resource2..\n");
  sleep(2);

  printf("Trying to get resourc1\n");
  pthread_mutex_lock(&lock1); 
  printf("Aquired resourc1\n");
  pthread_mutex_unlock(&lock1);

  printf("Job finished in resource2..\n");

  pthread_mutex_unlock(&lock2);

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
    
