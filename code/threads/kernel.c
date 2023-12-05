////////////////////////////////////////////////////
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strsafe.h>
#include <stdint.h>

#include <windows.h>
#endif
////////////////////////////////////////////////////

#include "../config.h"

#ifdef USE_LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#endif

#if defined(_WIN32)
#ifndef USE_CREATETHREAD
#include <process.h> // _beginthread, _endthread
#endif
#endif

///////////////////////////////////////////////////////////////
#if defined(_WIN32)
#define PTW32_STATIC_LIB
#define HAVE_STRUCT_TIMESPEC
#endif
///////////////////////////////////////////////////////////////
#include <pthread.h>
#include <sched.h>
///////////////////////////////////////////////////////////////

#include "../base/cstl.h"
#include "../base/types.h"
#include "../base/rnd.h"

#include "../threads/atomic.h"
#include "../threads/kernel.h"

/////////////////////////////////////////////

#ifdef USE_PTHREAD
static pthread_mutex_t g_kernel_mutex;
#else
static void* g_kernel_event;
#endif

/////////////////////////////////////////////

void kernel_sleep(int mili)
{
#ifdef _WIN32
	Sleep(mili);
	//SleepEx(mili, true);
#endif

#ifdef USE_LINUX
	usleep(mili * 1000); //sleep time as microseconds
#endif
}

void kernel_init()
{
#ifdef USE_PTHREAD
	pthread_mutex_init(&g_kernel_mutex, NULL);
#else
	// Create an auto reset auto-reset events, Trusted signaled Event kernel object 
	g_kernel_event = CreateEvent(NULL, FALSE, TRUE, NULL);
#endif
}

void kernel_lock()
{
#ifdef USE_PTHREAD
	pthread_mutex_lock(&g_kernel_mutex);
#else
	// Wait for the kernel event object state to be trusted
	WaitForSingleObject(g_kernel_event, INFINITE);
#endif
}

void kernel_free()
{
#ifdef USE_PTHREAD
	pthread_mutex_unlock(&g_kernel_mutex);
#else
	// Set trust
	SetEvent(g_kernel_event); 
#endif	
}

bool kernel_thread(void* thread_func, void* thread_data)
{
#ifndef USE_PTHREAD
	bool runs_immediately = true;
	uint32 threadid = 0;
#endif

#ifdef USE_CREATETHREAD
	void* h_thread = CreateThread(
		NULL, // Default security properties 
		0, // Default stack size 
		thread_func, // Thread entry address Function of executing thread
		thread_data, // Parameters passed to the function 
		(runs_immediately) ? 0 : CREATE_SUSPENDED, // Specifies that the thread runs immediately 
		&threadid); // Returns the thread's ID Number 
	if(h_thread != NULL)
		CloseHandle(h_thread);
#else
#ifdef USE_PTHREAD
	pthread_t pthrd;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&pthrd, &attr, thread_func, thread_data);
#else
	void* h_thread = (void*)_beginthreadex(NULL, 0, thread_func, thread_data, (runs_immediately) ? 0 : CREATE_SUSPENDED, &threadid);
	if(h_thread != NULL)
		CloseHandle(h_thread);
#endif
#endif // USE_CREATETHREAD	
	return true;
}

/*
uint32 kernel_thread_run(void* thread)
{
	return ResumeThread(thread);
}

uint32 kernel_thread_pause(void* thread)
{
	return SuspendThread(thread);
}

void kernel_thread_free(void* thread)
{
	CloseHandle(thread);
}

void kernel_thread_wait_end(void* thread, uint32 milliseconds) //until end use INFINITE for milliseconds
{
	WaitForSingleObject(thread, milliseconds);
}

void kernel_threads_wait_all_end(void* threads, uint32 threads_count, uint32 milliseconds) //until end use INFINITE for milliseconds
{
	WaitForMultipleObjects(threads_count, threads, true, milliseconds);
}

uint32 kernel_current_threadid()
{
	return GetCurrentThreadId();
}
*/
/////////////////////////////////////////////