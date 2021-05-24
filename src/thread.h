#ifndef _THREAD_H_
#define _THREAD_H_

#define	COUNT_KEY	0
#define YES		1
#define NO		2
#define	UNU	__attribute__((unused))

#define	PI_THREAD(X)	void *X (UNU void *dummy)


void busyWait(int ms);
void startThread(void);
int checkThreadResult(void);

#endif
