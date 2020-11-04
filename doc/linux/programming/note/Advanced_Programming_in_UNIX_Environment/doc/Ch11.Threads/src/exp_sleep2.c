#include <pthread.h>
#include <stdio.h>

// Simplest way to create a mutex, no function calls needed
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

// When two threads must use the same memory, you must use
// mutex calls to make it safe.  It's not just a lock, it's also a
// "memory barrier", guaranteeing memory values between different CPU cores
// are synchronized properly.
int running=1;

void *thread(void *arg) {
  fprintf(stderr, "Thread beginning\n");

  while(1) {
    int r;
    fprintf(stderr, "thread waiting for lock\n");
    // If main already has the mutex, thread will sleep.
    // when main unlocks it, the thread will lock it and continue.
    pthread_mutex_lock(&lock);
    fprintf(stderr, "We have the mutex, it is safe to get value of 'running'\n");
    r=running;
    fprintf(stderr, "Value of 'running' is %d\n", r);
    pthread_mutex_unlock(&lock); // What gets locked MUST be unlocked later!
    fprintf(stderr, "thread has given up the lock\n");
    if(r == 0) break;
    sleep(1);

  }
  fprintf(stderr, "thread finishing\n");

  return((void *)0xdeadbeef);

}

int main() {
  void *ret;
  pthread_t tid;
  running=1;
  pthread_mutex_lock(&lock); // Stop thread from getting the mutex
  fprintf(stderr, "main now has the mutex\n");

  pthread_create(&tid, NULL, thread, NULL);
  sleep(2); // Thread will begin, but wait for mutex

  fprintf(stderr, "main is unlocking the mutex\n");
  pthread_mutex_unlock(&lock); // Let the thread run for a while
  sleep(5);

  // Lock the mutex, making it safe to alter 'running'
  pthread_mutex_lock(&lock);
  fprintf(stderr, "main now has the mutex\n");
  running=0;
  fprintf(stderr, "main is unlocking the mutex\n");
  pthread_mutex_unlock(&lock);

  pthread_join(tid, &ret);
  fprintf(stderr, "Thread finished with return value %p\n", ret);

}
