#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#define	SEM_A	0200	/* alter permission */
#define	SEM_R	0400	/* read permission */
#define	SHM_W	0200	/* write permission */
#define	SHM_R	0400	/* read permission */
#define N 5     
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];
int semgroup;

void philosopher(int );
void think(int );
void eat(int );
void test(int );
void put_spoons(int );
void take_spoons(int );
void initialize_shared();
void up(int );
void down(int );

// Initialisation of shared memory

struct shm {
	int state[N];
} *shared;

void initialize_shared()
{	
        int i;	int shmid; 
	
        shmid = shmget(IPC_PRIVATE, sizeof(*shared),IPC_CREAT|SHM_W|SHM_R);
	printf("shmid: %i \n", shmid);
	shared = (struct shm *)  shmat(shmid, (char *)0, 0);
	semgroup = semget(IPC_PRIVATE, N+1, IPC_CREAT|SEM_A|SEM_R);
	printf("semgroup: %i \n", semgroup);
	for (i=0; i<N; i++) {
	        up(i);
		shared->state[i] = THINKING;
	}up(5);
        printf("end of initialisation of semaphores\n");
}

/* Dijkstra's down */

void down(int semid) {
	struct sembuf semopr;

	semopr.sem_num = semid;
	semopr.sem_op = -1;
	semopr.sem_flg = 0;
	semop(semgroup, &semopr, 1);

}

/* Dijkstra's up */
void up(int semid) {
	struct sembuf semopr;

	semopr.sem_num = semid;
	semopr.sem_op = 1;
	semopr.sem_flg = 0;
	semop(semgroup, &semopr, 1);
}


// The Tennenbaum’s solution

void take_spoon(int i)
{
  down(5);
  shared->state[i] = HUNGRY;
  printf("philosopher %d is hungry\n",i+1);
  test(i);
  up(5);
  down(i);
}

void put_spoon(int i)
{
  down(5);
  shared->state[i] = THINKING;
  printf("philosopher %d puts down spoon while %d and %d thinks\n",i+1,LEFT+1,i+1);
  printf("philosopher %d thinks\n",i+1);
  test(LEFT);
  test(RIGHT);
  up(5);
}

void test(int i)
{
  if( shared->state[i] == HUNGRY && shared->state[LEFT] != EATING && shared->state[RIGHT] != EATING)
  {
    shared->state[i] = EATING;
    printf("philosopher %d takes spoon and eats while %d and %d thinks\n",i+1,LEFT+1,i+1);
    printf("philosopher %d eats\n",i+1);
    up(i);
  }
}

void think (int i)
{
//
}

void eat (int i)
{
//
}
void philosopher(int i)
{
  while(1)
  {
    think(i);
    sleep(1);
    take_spoon(i);
    sleep(2);
    eat(i);
    sleep(2);
    put_spoon(i);
    sleep(1);
  }
}

//Main Function

int main(void)
{
  int i;
  pid_t pid, pids[N]; // process ids
  printf("Starting initialisation processes\n");
  initialize_shared(); //initialisation of the semaphores before starting
  for(i=0;i<N;++i)
  {
    pid = fork();
    if(pid==0)
    {
      // child
      philosopher(i);
      //_exit(0);
    }
    else if(pid>0)
    {
      // parent
      pids[i] = pid;
      printf("pids[%d]=%d\n",i,pids[i]);
    }
    else
    {
      perror("fork");
      _exit(0);
    }
  }
  
}




























