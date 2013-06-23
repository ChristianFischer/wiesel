/**
 * Copyright (C) 2012
 * Christian Fischer
 *
 * https://bitbucket.org/baldur/wiesel/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */
#include "thread.h"

#if linux
#include <unistd.h>
#endif

using namespace wiesel;


IRunnable::IRunnable() {
	return;
}

IRunnable::~IRunnable() {
	return;
}




#if WIESEL_THREADAPI_PTHREAD
	static void* pthread_entry(void* arg) {
		Thread *thread = reinterpret_cast<Thread*>(arg);
		_thread_impl(thread);

		return NULL;
	}
#endif // WIESEL_THREADAPI_PTHREAD


#if WIESEL_THREADAPI_WIN32
	static DWORD WINAPI win32_thread_entry(void* arg) {
		Thread *thread = reinterpret_cast<Thread*>(arg);
		_thread_impl(thread);

		return 1;
	}
#endif // WIESEL_THREADAPI_WIN32




Thread::Thread() {
	this->runnable	= NULL;
	init();
}

Thread::Thread(IRunnable* runnable) {
	this->runnable	= keep(runnable);
	init();
}

Thread::~Thread() {
	assert(this->state == None);

	#if WIESEL_THREADAPI_PTHREAD
		if (thread_mutex != NULL) {
			pthread_mutex_destroy(thread_mutex);
			thread_mutex  = NULL;
		}
	#elif WIESEL_THREADAPI_WIN32
		if (critical_section != NULL) {
			DeleteCriticalSection(critical_section);
			delete critical_section;
			critical_section = NULL;
		}
	#endif

	clear_ref(runnable);

	return;
}


void Thread::init() {
	this->state		= None;

	#if WIESEL_THREADAPI_PTHREAD
		this->thread_mutex		= new pthread_mutex_t();
		*(this->thread_mutex)	= PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_init(thread_mutex, NULL);
	#elif WIESEL_THREADAPI_WIN32
		this->thread_handle		= 0;
		this->critical_section	= new CRITICAL_SECTION();
		InitializeCriticalSection(critical_section);
	#endif

	return;
}


void Thread::sleep(uint32_t ms) {
	#if linux
		usleep(ms * 1000);
	#elif WIN32
	    Sleep(ms);
	#else
		#error cannot sleep
	#endif
}


bool Thread::start() {
	assert(state == None);
	
	if (state == None) {
		state  = Running;

		// the running thread keeps a reference to itself
		keep(this);

		#if WIESEL_THREADAPI_PTHREAD
			pthread_create(&thread_handle, NULL, &pthread_entry, reinterpret_cast<void*>(this));
		#elif WIESEL_THREADAPI_WIN32
			thread_handle = CreateThread(NULL, 0, &win32_thread_entry, reinterpret_cast<void*>(this), 0, NULL);
			if (thread_handle == NULL) {
				return false;
			}
		#else
			#error no valid thread-API configured.
		#endif

		return true;
	}

	return false;
}


void Thread::detach() {
	assert(state == Running || state == WaitingForJoin);

	if (state == Running || state == WaitingForJoin) {
		lock();

		#if WIESEL_THREADAPI_PTHREAD
			pthread_detach(thread_handle);
		#elif WIESEL_THREADAPI_WIN32
			CloseHandle(thread_handle);
			thread_handle = NULL;
		#else
			#error no valid thread-API configured.
		#endif

		switch(state) {
			case Running: {
				state = RunningDetached;
				break;
			}

			case WaitingForJoin: {
				state = None;
				break;
			}

			default: {
				break;
			}
		}

		unlock();
	}

	return;
}


void Thread::join() {
	assert(state == Running || state == WaitingForJoin);

	if (state == Running || state == WaitingForJoin) {
		#if WIESEL_THREADAPI_PTHREAD
			pthread_join(thread_handle, NULL);
		#elif WIESEL_THREADAPI_WIN32
			WaitForSingleObject(thread_handle, INFINITE);
			CloseHandle(thread_handle);
			thread_handle = NULL;
		#else
			#error no valid thread-API configured.
		#endif

		state = None;

		// release the reference to the running thread
		release(this);
	}

	return;
}


bool Thread::isRunning() const {
	switch(state) {
		case Running:
		case RunningDetached:
			return true;

		default:
			break;
	}

	return false;
}


void Thread::lock() {
	#if WIESEL_THREADAPI_PTHREAD
		pthread_mutex_lock(thread_mutex);
	#else
		EnterCriticalSection(critical_section);
	#endif

	// locking a thread increments the reference counter
	keep(this);

	return;
}


void Thread::unlock() {
	assert(getReferenceCount() >= 1);

	// releasing this lock will destroy our object,
	// so we need to keep the mutex handle first
	bool object_will_be_deleted = false;
	if (getReferenceCount() <= 1) {
		object_will_be_deleted = true;
	}

	#if WIESEL_THREADAPI_PTHREAD
		pthread_mutex_t *mutex = this->thread_mutex;

		if (object_will_be_deleted) {
			this->thread_mutex = NULL;
		}
	#elif WIESEL_THREADAPI_WIN32
		LPCRITICAL_SECTION cs = this->critical_section;

		if (object_will_be_deleted) {
			this->critical_section = NULL;
		}
	#endif

	// clear the reference obtained by this lock
	release(this);

	#if WIESEL_THREADAPI_PTHREAD
		pthread_mutex_unlock(mutex);

		if (object_will_be_deleted) {
			pthread_mutex_destroy(mutex);
			delete mutex;
		}
	#elif WIESEL_THREADAPI_WIN32
		LeaveCriticalSection(cs);

		if (object_will_be_deleted) {
			LeaveCriticalSection(cs);
			delete cs;
		}
	#else
		#error no valid thread-API configured.
	#endif

	return;
}



void Thread::run() {
	if (runnable) {
		runnable->run();
	}

	return;
}



void wiesel::_thread_impl(Thread *thread) {
	assert(thread->state == Thread::Running || thread->state == Thread::RunningDetached);

	thread->run();

	{
		thread->lock();

		switch(thread->state) {
			case Thread::Running: {
				thread->state = Thread::WaitingForJoin;
				break;
			}

			case Thread::RunningDetached: {
				thread->state = Thread::None;
				release(thread);
				break;
			}

			default: {
				break;
			}
		}

		thread->unlock();
	}

	return;
}
