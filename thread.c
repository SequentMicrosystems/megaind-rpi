#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>

#include "thread.h"


static pthread_mutex_t piMutexes [4];

int piHiPri (const int pri);
int piThreadCreate (void *(*fn)(void *));
static volatile int globalResponse = 0;

PI_THREAD (waitForKey)
{
 char resp;
 int respI = NO;

 
	struct termios info;
	tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
	info.c_lflag &= ~ICANON;      /* disable canonical mode */
	info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
	info.c_cc[VTIME] = 0;         /* no timeout */
	tcsetattr(0, TCSANOW, &info); /* set i */

	(void)piHiPri (10) ;	// Set this thread to be high priority
	resp = getchar();
	if((resp == 'y')||(resp == 'Y'))
	{
		respI = YES;
	}
	
    pthread_mutex_lock(&piMutexes[COUNT_KEY]);
	globalResponse = respI;
    pthread_mutex_unlock(&piMutexes[COUNT_KEY]);
	
	info.c_lflag |= ICANON;      /* disable canonical mode */
	info.c_cc[VMIN] = 0;          /* wait until at least one keystroke available */
	info.c_cc[VTIME] = 0;         /* no timeout */
	tcsetattr(0, TCSANOW, &info); /* set i */
	printf("\n");
	return &waitForKey;
}

/*
 * upHiPri:
 *	Attempt to set a high priority scheduling for the running program
 *********************************************************************************
 */

int piHiPri (const int pri)
{
  struct sched_param sched ;

  memset (&sched, 0, sizeof(sched)) ;

  if (pri > sched_get_priority_max (SCHED_RR))
    sched.sched_priority = sched_get_priority_max (SCHED_RR) ;
  else
    sched.sched_priority = pri ;

  return sched_setscheduler (0, SCHED_RR, &sched) ;
}

/*
 * upThreadCreate:
 *	Create and start a thread
 *********************************************************************************
 */

int piThreadCreate (void *(*fn)(void *))
{
  pthread_t myThread ;

  return pthread_create (&myThread, NULL, fn, NULL) ;
}

void startThread(void)
{
	piThreadCreate(waitForKey);
}

int checkThreadResult(void)
{
	int res;
	pthread_mutex_lock(&piMutexes[COUNT_KEY]);
	res = globalResponse;
	pthread_mutex_unlock(&piMutexes[COUNT_KEY]);
	return res;
}

/*
 * busyWait:
 *	Wait for some number of milliseconds
 *********************************************************************************
 */

void busyWait(int ms)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(ms / 1000) ;
  sleeper.tv_nsec = (long)(ms % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}
