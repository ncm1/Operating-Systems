#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>

void *thread_function(void *param){

  int *incoming = (int *)param;
  //Do whatever is neccessary
  for(int i = 0; i < 5; i++){
  printf("Hello world : [thread %d]\n", *incoming);
  }

  pid_t thread_id = syscall(__NR_gettid);
  printf("thread_id: %d\n", thread_id);
  //The thread terminates when this function returns
  return NULL;
}

int main(int argc, char* argv[]){
pthread_t thread_ID[10];
void      *thread_result;

for(int i = 1; i <= 11; i++){
  pthread_create(&thread_ID[i],NULL,thread_function,&i);
  pthread_join(thread_ID[i], &thread_result);
  printf("----------------------------\n");
}

return 0;
}
