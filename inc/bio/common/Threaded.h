/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2022 Séon O'Shannon & eons LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "ThreadSafe.h"
#include "bio/common/macros/LanguageMacros.h"
#include "bio/common/macros/OSMacros.h"
#include "Types.h"
#include "bio/common/macros/Macros.h"

//@formatter:off
#if BIO_CPP_VERSION < 11
	#ifdef BIO_OS_IS_LINUX
		#include <pthread.h>
	#endif
#else
	#include <thread>
#endif
//@formatter:on

namespace bio {

/**
 * Threaded classes are a wrapper around whatever thread interface the system is using. <br />
 * This class moves whatever Work() you need to do into a new thread with a simple wrapper that is OS & c++ version independent. <br />
 *
 * NOTE: we're currently only supporting threading on c++11 and greater and on linux using c++98 on. <br />
 * We may or may not ever support c++98 threading on windows, etc. <br />
 * TODO: does pthread work on Apple and BSD? <br />
 *
 * NOTE: YOU MUST CALL STOP BEFORE DESTROYING *this!!!! <br />
 */
class Threaded :
	virtual public ThreadSafe
{
public:
	#if BIO_CPP_VERSION < 11
	typedef pid_t ThreadId;
	#else
	typedef std::thread::id ThreadId;
	#endif

	/**
	 * @return what is hopefully not a proper ThreadId.
	 */
	static ThreadId InvalidThreadId()
	{
		return 0;
	}

	/**
	 * YOU MUST CALL STOP BEFORE DESTROYING *this!!!! <br />
	 */
	Threaded();

	//TODO: support move & copy ctors.

	/**
	 *
	 */
	virtual ~Threaded();

	/**
	 * Does the actual work. <br />
	 * Will be called repeatedly until either: Stop() is called OR this method returns false. <br />
	 * So, just return false when you want to stop being a thread. <br />
	 * You may want to Sleep() (below) after you Work() ;) <br />
	 * @return false when you want to stop; true if you want to run until Stop()ed.
	 */
	virtual bool Work()
	{
		//your code goes here!
		return false;
	}

	/**
	 * @return the id of our thread.
	 */
	virtual ThreadId GetThreadId();

	/**
	 * Starts our thread, which will continuously call Work() until Stop()ed (or Work() exits). <br />
	 * @return whether or not our thread was successfully created.
	 */
	virtual bool Start();

	/**
	 * Instructs our thread to stop calling Work() and joins our thread. <br />
	 * @return whether or not our thread was successfully joined.
	 */
	virtual bool Stop();

	/**
	 * @return whether or not Work() is being called by our thread.
	 */
	virtual bool IsRunning();

	/**
	 * Release thread processing for us microseconds. <br />
	 * @param us
	 */
	virtual void Sleep(TimeUS us);

protected:

	//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			pthread_t m_thread;
		#endif
		ThreadId m_id;
	#else
		std::thread* m_thread; 
	#endif
	//@formatter:on

	/**
	 * Sets m_stopRequested to true. <br />
	 */
	virtual void RequestStop();

	bool m_created;
	bool m_running; //written by spawn; read by parent.
	bool m_stopRequested; //written by parent; read by spawn.

	/**
	 * @param arg a Threaded*
	 * @return NULL
	 */
	static void* Worker(void* arg); 
};

} //bio namespace
