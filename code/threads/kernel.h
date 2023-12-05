/////////////////////////////////////////////
// kernel threads and lock/unlock
/////////////////////////////////////////////
#ifdef USE_CREATETHREAD
#define KERNEL_THREAD_FUNC(fnc) DWORD WINAPI fnc(LPVOID p_data)
#else
#ifdef USE_PTHREAD
#define KERNEL_THREAD_FUNC(fnc) void* fnc(void* p_data)
#else
#define KERNEL_THREAD_FUNC(fnc) int __stdcall fnc(void* p_data)
#endif
#endif

void kernel_sleep(int mili);

void kernel_init();

void kernel_lock();
void kernel_free();

bool kernel_thread(void* thread_func, void* thread_data);
//uint32 kernel_thread_run(void* thread);
//uint32 kernel_thread_pause(void* thread);

//void kernel_thread_wait_end(void* thread, uint32 milliseconds); //until end use INFINITE for milliseconds
//void kernel_threads_wait_all_end(void* threads, uint32 threads_count, uint32 milliseconds); //until end use INFINITE for milliseconds

//void kernel_thread_free(void* thread);

//uint32 kernel_current_threadid();
/////////////////////////////////////////////
