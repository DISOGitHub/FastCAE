#ifndef __FAST_MUTEX_HPP__
#define __FAST_MUTEX_HPP__

#ifndef _WIN32

/* this is done slightly differently on bsd-variants */
#if defined(__FreeBSD__) ||  defined(__APPLE_CC__) || defined(__OpenBSD__)
#define recursive_mutex_flag PTHREAD_MUTEX_RECURSIVE
#else
#define recursive_mutex_flag PTHREAD_MUTEX_RECURSIVE_NP
#endif

#endif

class Mutex
{
public:
	///friend class Condition;

	/** Initializes a mutex class, with InitializeCriticalSection / pthread_mutex_init
	 */
	Mutex()
    {
#ifdef _WIN32
        InitializeCriticalSection(&cs);
#else
        if(!attr_initalized) {
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, recursive_mutex_flag);
            attr_initalized = true;
        }

        pthread_mutex_init(&mutex, &attr);
#endif        
    }

	/** Deletes the associated critical section / mutex
	 */
	virtual ~Mutex()
    {
#ifdef _WIN32
        DeleteCriticalSection(&cs);
#else
        pthread_mutex_destroy(&mutex);
#endif        
    }

	/** Acquires this mutex. If it cannot be acquired immediately, it will block.
	 */
	inline void Acquire()
	{
#ifndef _WIN32
		pthread_mutex_lock(&mutex);
#else
		EnterCriticalSection(&cs);
#endif
	}

	/** Releases this mutex. No error checking performed
	 */
	inline void Release()
	{
#ifndef _WIN32
		pthread_mutex_unlock(&mutex);
#else
		LeaveCriticalSection(&cs);
#endif
	}

	/** Attempts to acquire this mutex. If it cannot be acquired (held by another thread)
	 * it will return false.
	 * @return false if cannot be acquired, true if it was acquired.
	 */
	inline bool AttemptAcquire()
	{
#ifndef _WIN32
		return (pthread_mutex_trylock(&mutex) == 0);
#else
		return (TryEnterCriticalSection(&cs) == TRUE ? true : false);
#endif
	}

protected:
#ifdef _WIN32
	/** Critical section used for system calls
	 */
	CRITICAL_SECTION cs;

#else
	/** Static mutex attribute
	 */
	static bool attr_initalized;
	static pthread_mutexattr_t attr;

	/** pthread struct used in system calls
	 */
	pthread_mutex_t mutex;
#endif
};

#ifdef _WIN32

class FastMutex
{
#pragma pack(push,8)
	volatile long m_lock;
#pragma pack(pop)
	DWORD m_recursiveCount;

public:
	inline FastMutex() : m_lock(0),m_recursiveCount(0) {}
	inline ~FastMutex() {}

	inline void Acquire()
	{
		DWORD thread_id = GetCurrentThreadId(), owner;
		if(thread_id == (DWORD)m_lock)
		{
			++m_recursiveCount;
			return;
		}

		for(;;)
		{
			owner = InterlockedCompareExchange(&m_lock, thread_id, 0);
			if(owner == 0)
				break;

			Sleep(0);
		}

		++m_recursiveCount;
	}

	inline bool AttemptAcquire()
	{
		DWORD thread_id = GetCurrentThreadId();
		if(thread_id == (DWORD)m_lock)
		{
			++m_recursiveCount;
			return true;
		}

		DWORD owner = InterlockedCompareExchange(&m_lock, thread_id, 0);
		if(owner == 0)
		{
			++m_recursiveCount;
			return true;
		}

		return false;
	}

	inline void Release()
	{
		if((--m_recursiveCount) == 0)
			InterlockedExchange(&m_lock, 0);
	}
};

#else
/* Linux mutex implementation */
bool Mutex::attr_initalized = false;
pthread_mutexattr_t Mutex::attr;

#define FastMutex Mutex

#endif

#endif
