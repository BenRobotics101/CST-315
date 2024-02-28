
// Second deadlock scenario. Two mutexes are used, read_mutex and write_mutex. 
// To gain access to the file both these mutexes have to be locked. In the code, 
// the write thread locks write_mutex and then after a little delay tries to locks read_mutex. 
// The read thread on the other hand locks read_mutex first and then tries to 
// lock write_mutex. Thus by the time the write tries to access read_mutex, it has already 
// been locked by read thread and by the time read thread tries to lock write_mutex, it has already 
// been locked by the write thread. Both the threads end up waiting infinitely for each other to 
// unlock the mutexes and process ends up in a deadlock. 

#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <unistd.h>
pthread_mutex_t file_mutex;

void * write_func(void *temp) 
{
  char *ret;
  FILE *file1;
  char *str;
  pthread_mutex_lock(&file_mutex);
  printf("\nFile locked, please enter the message \n\n");
  str=(char *)malloc(10*sizeof(char));
  file1=fopen("temp.txt","w");
  scanf("%s",str);
  fprintf(file1,"%s",str);
  fclose(file1);
  pthread_mutex_unlock(&file_mutex);
  printf("\nUnlocked the file you can read it now \n");
  return ret;
}


void * read_func(void *temp) 
{
  char *ret;
  FILE *file1;
  char *str;
  pthread_mutex_lock(&file_mutex);
  printf("\nOpening file \n");
  file1=fopen("temp.txt","r");
  str=(char *)malloc(10*sizeof(char));
  fscanf(file1,"%s",str);
  printf("\nMessage from file is %s \n",str);

  fclose(file1);

  pthread_mutex_unlock(&file_mutex);
  return ret;
}

int main() 
{
  pthread_t thread_id,thread_id1;
  pthread_attr_t attr;
  int ret;
  void *res;
  ret=pthread_create(&thread_id,NULL,&write_func,NULL);
  ret=pthread_create(&thread_id1,NULL,&read_func,NULL);
  printf("\nCreated thread \n");
  pthread_join(thread_id,&res);
  pthread_join(thread_id1,&res);
  return 0;
}
    
