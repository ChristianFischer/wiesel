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
#ifndef __WIESEL_UTIL_THREAD_H__
#define	__WIESEL_UTIL_THREAD_H__

#include "shared_object.h"

#include "wiesel/wiesel-base-config.h"

#if WIESEL_THREADAPI_PTHREAD
#	include <pthread.h>
#endif

#if WIESEL_THREADAPI_WIN32
#	include <windows.h>
#	include <process.h>
#endif

namespace wiesel {

	class Thread;
	void _thread_impl(Thread *thread);


	/**
	 * @brief A interface class for any object which should perform a single task.
	 */
	class WIESEL_BASE_EXPORT IRunnable : public virtual SharedObject
	{
	public:
		IRunnable();
		virtual ~IRunnable();

	public:
		/**
		 * @brief Performs the task of this object.
		 */
		virtual void run() = 0;
	};


	/**
	 * @brief A class managing a single threaded process.
	 */
	class WIESEL_BASE_EXPORT Thread : public IRunnable
	{
	friend void _thread_impl(Thread *thread);

	protected:
		Thread();

	public:
		/**
		 * @brief Creates a new thread with a reference to a single \ref IRunnable object.
		 * The task of this object will be executed in a seperate thread.
		 */
		Thread(IRunnable *runnable);

		virtual ~Thread();

	private:
		void init();

	public:
		/**
		 * @brief Starts this thread.
		 * After starting, the caller should call either
		 * \ref join() or \ref detach()
		 * @return \c true on success.
		 */
		bool start();

		/**
		 * @brief Detaches this thread.
		 */
		void detach();

		/**
		 * @brief Wait for this thread until it has been finished.
		 */
		void join();

		/**
		 * @brief Check if this thread is still running.
		 */
		bool isRunning() const;

	// locking
	public:
		void lock();
		void unlock();

	// IRunnable
	public:
		/**
		 * @brief This function will be called, when the thread was started.
		 * To be implemented by subclasses.
		 */
		virtual void run();

	private:
		enum State {
			None,
			Running,
			RunningDetached,
			WaitingForJoin,
		};

		IRunnable*		runnable;
		State			state;

	// api specific
	private:
		#if WIESEL_THREADAPI_PTHREAD
			pthread_t			thread_handle;
			pthread_mutex_t*	thread_mutex;
		#elif WIESEL_THREADAPI_WIN32
			HANDLE				thread_handle;
			LPCRITICAL_SECTION	critical_section;
		#endif
	};
}

#endif	// __WIESEL_UTIL_THREAD_H__
