 /* LiSA - Library for System Adaptation
  * Copyright (C) 2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public
  * License along with this library; if not, write to the Free
  * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  * MA 02111-1307, USA */

#include <lisa.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void LiSAInit()
{
}

void LiSADeinit()
{
}

void LiSASleep(int mSeconds)
{
	if (mSeconds == 0) return;

	usleep(mSeconds * 1000);
}

int LiSASetMouseCursor(int mouseCursor)
{
	if (mouseCursor == LiSA_MOUSE_ARROW)	return 0;
	else					return 1;
}

int LiSAOpenURLA(char *url)
{
	return 1;
}

int LiSAOpenURLW(wchar_t *url)
{
	return 1;
}

int LiSAGetDisplaySizeX()
{
	return 0;
}

int LiSAGetDisplaySizeY()
{
	return 0;
}

LiSAThread *LiSAThreadCreate(void (*threadProc)(void *), void *threadParam)
{
	pthread_t	*thread = new pthread_t;

	pthread_create(thread, NULL, (void *(*)(void *)) threadProc, threadParam);

	return (LiSAThread *) thread;
}

int LiSAThreadCancel(LiSAThread *thread)
{
	pthread_cancel(*((pthread_t *) thread));

	return 0;
}

void LiSAThreadExit()
{
	pthread_exit(0);
}

int LiSAThreadCloseHandle(LiSAThread *thread)
{
	delete (pthread_t *) thread;

	return 0;
}

LiSAThread *LiSAThreadSelf()
{
	pthread_t	*thread = NULL;

	*thread = pthread_self();

	return (LiSAThread *) thread;
}

LiSASemaphore *LiSASemaphoreCreate()
{
	sem_t	*semaphore = new sem_t;

	sem_init(semaphore, 0, 1);

	return (LiSASemaphore *) semaphore;
}

int LiSASemaphoreWait(LiSASemaphore *semaphore)
{
	sem_wait((sem_t *) semaphore);

	return 0;
}

int LiSASemaphoreRelease(LiSASemaphore *semaphore)
{
	sem_post((sem_t *) semaphore);

	return 0;
}

int LiSASemaphoreCloseHandle(LiSASemaphore *semaphore)
{
	sem_destroy((sem_t *) semaphore);

	delete (sem_t *) semaphore;

	return 0;
}

LiSAMutex *LiSAMutexCreate()
{
	pthread_mutex_t	*mutex = new pthread_mutex_t;

	pthread_mutex_init(mutex, NULL);

	return (LiSAMutex *) mutex;
}

int LiSAMutexLock(LiSAMutex *mutex)
{
	pthread_mutex_lock((pthread_mutex_t *) mutex);

	return 0;
}

int LiSAMutexRelease(LiSAMutex *mutex)
{
	pthread_mutex_unlock((pthread_mutex_t *) mutex);

	return 0;
}

int LiSAMutexCloseHandle(LiSAMutex *mutex)
{
	pthread_mutex_destroy((pthread_mutex_t *) mutex);

	delete (pthread_mutex_t *) mutex;

	return 0;
}
