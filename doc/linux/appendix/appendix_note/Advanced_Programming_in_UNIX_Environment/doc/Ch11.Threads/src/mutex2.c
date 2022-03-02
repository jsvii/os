#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(fp) (((unsigned long)fp)%NHASH)

struct foo *fh[NHASH];

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
  int f_count;
  pthread_mutex_t f_lock;
  struct foo *f_next; /* protected by hashlock */
  int f_id;
  /* ... more stuff here ... */
};

/* 新生成一个元素，往 */
struct foo *
foo_alloc(void) /* allocate the object */
{
  struct foo *fp;
  int idx;

  if ((fp = malloc(sizeof(struct foo))) != NULL) {
    fp->f_count = 1;
    if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
      free(fp);
      return(NULL);
    }
    idx = HASH(fp);
    pthread_mutex_lock(&hashlock);
    fp->f_next = fh[idx];   // idx为新位置，把原来元素的位置变为next
    fh[idx] = fp;   // 新元素放入
    pthread_mutex_lock(&fp->f_lock); // 关键点：解锁hashlock前，先锁住新元素的锁，因此此时不希望其他线程进入，拿到这个元素
    pthread_mutex_unlock(&hashlock);
    /* ... continue initialization ... */
    pthread_mutex_unlock(&fp->f_lock);
  }
  return(fp);
}

void
foo_hold(struct foo *fp) /* add a reference to the object */
{
  pthread_mutex_lock(&fp->f_lock);
  fp->f_count++;
  pthread_mutex_unlock(&fp->f_lock);
}

struct foo *
foo_find(int id) /* find an existing object */
{
  struct foo *fp;
  int idx;

  idx = HASH(fp);
  pthread_mutex_lock(&hashlock);
  for (fp = fh[idx]; fp != NULL; fp = fp->f_next) {
    if (fp->f_id == id) {
      foo_hold(fp);
      break;
    }
  }
  pthread_mutex_unlock(&hashlock);
  return(fp);
}

void
foo_rele(struct foo *fp) /* release a reference to the object */
{
  struct foo *tfp;
  int idx;

  pthread_mutex_lock(&fp->f_lock);
  if (fp->f_count == 1) { /* last reference */
    pthread_mutex_unlock(&fp->f_lock);
    pthread_mutex_lock(&hashlock);
    pthread_mutex_lock(&fp->f_lock);
    /* need to recheck the condition */
    if (fp->f_count != 1) {
      fp->f_count--;
      pthread_mutex_unlock(&fp->f_lock);
      pthread_mutex_unlock(&hashlock);
      return;
    }
    /* remove from list */
    idx = HASH(fp);
    tfp = fh[idx];
    if (tfp == fp) {
      fh[idx] = fp->f_next;
    } else {
      while (tfp->f_next != fp)
        tfp = tfp->f_next;
      tfp->f_next = fp->f_next;
    }
    pthread_mutex_unlock(&hashlock);
    pthread_mutex_unlock(&fp->f_lock);
    pthread_mutex_destroy(&fp->f_lock);
    free(fp);
  } else {
    fp->f_count--;
    pthread_mutex_unlock(&fp->f_lock);
  }
}
