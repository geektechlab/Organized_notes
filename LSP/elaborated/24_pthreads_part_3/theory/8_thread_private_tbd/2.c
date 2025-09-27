#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int          threadSpecific1;
  int          threadSpecific2;
} threadSpecific_data_t;

#define                 NUMTHREADS   2
pthread_key_t           threadSpecificKey;

void dataDestructor(void *data) {
   printf("Thread %.8x %.8x: Free data\n", pthread_self());
   pthread_setspecific(threadSpecificKey, NULL);
   free(data);
}

void bar() {
   threadSpecific_data_t *gData = pthread_getspecific(threadSpecificKey);
   printf("Thread %.8x %.8x: bar(), threadSpecific data=%d %d\n",
          pthread_self(), gData->threadSpecific1, gData->threadSpecific2);
   return;
}


void foo() {
   threadSpecific_data_t *gData = pthread_getspecific(threadSpecificKey);
   printf("Thread %.8x %.8x: foo(), threadSpecific data=%d %d\n",
          pthread_self(), gData->threadSpecific1, gData->threadSpecific2);
   bar();
}


void *theThread(void *parm)
{
   int               rc;
   threadSpecific_data_t    *gData;
   printf("Thread %.8x %.8x: Entered\n", pthread_self());
   gData = (threadSpecific_data_t *)parm;
   rc = pthread_setspecific(threadSpecificKey, gData);
   foo();
   return NULL;
}


int main(int argc, char **argv)
{
  pthread_t             thread[NUMTHREADS];
  int                   rc=0;
  int                   i;
  threadSpecific_data_t        *gData;


    rc = pthread_key_create(&threadSpecificKey, dataDestructor);
printf("Create/start threads\n");
  for (i=0; i <NUMTHREADS; ++i) {
       /* Create per-thread threadSpecific data and pass it to the thread */
     gData = (threadSpecific_data_t *)malloc(sizeof(threadSpecific_data_t));
     gData->threadSpecific1 = i;
     gData->threadSpecific2 = (i+1)*2;
     rc = pthread_create(&thread[i], NULL, theThread, gData);
  }

  printf("Wait for the threads to complete, and release their resources\n");
  for (i=0; i <NUMTHREADS; ++i) {
  rc = pthread_join(thread[i], NULL);
  }

  pthread_key_delete(threadSpecificKey);
  printf("Main completed\n");
  return 0;
}
