#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SIZE 10

int Counter;
struct Stack
{
  int home[MAX_SIZE];
  int top;

};
struct Stack s;

void InitStack(struct Stack *s) {
  s->top = -1;
  Counter = 0;
}

void Push(struct Stack *s, int x) {
  if(s->top == MAX_SIZE-1) {
    printf("Stack is full!\n");
    exit(0);
  } else {
    s->top++;
  }

  s->home[s->top] = x;
  Counter++;
  printf("Counter = %d\n", Counter);
}

void Pop(struct Stack *s) {
  int y;
  if(s->top == -1) {
    printf("Stack empty!\n");
    exit(0);

  }
  else {
    y = s->home[s->top];
  }

  s->top--;
  printf("%d poped\n",y);
  Counter--;
  printf("Counter = %d\n", Counter);
}

void *threadA(void *);
void *threadB(void *);
int main() {
  InitStack(&s);
  int i;
  pthread_t tid[2];
  pthread_attr_t attr[2];

  for(i=1;i<=2;i++) {
    pthread_attr_init(&attr[i-1]);
  }

  {
    pthread_create(&tid[0],&attr[0],threadA,(void *)786);
    pthread_create(&tid[1],&attr[1],threadB,(void *)786);
  }

  {
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);

  }
  return 0;

}

void *threadA(void *n) {
  int x;
  x = (int)n;

  while(1) {
    if(Counter != MAX_SIZE) {
      Push(&s,x);
      printf("x = %d\n", x);
      sleep(1);
    } else {
      printf("Going for a long sleep as Stack is full!\n");
      sleep(50);
    }
  }
}

void *threadB(void *n) {
  sleep(15);

  while(1) {
    if(Counter != 0) {
      Pop(&s);
      sleep(1);
    } else {
      printf("Going for a long sleep as Stack is empty!\n");
      sleep(50);
    }
  }
}
