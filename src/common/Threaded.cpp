/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/common/Threaded.h"
#include "bio/common/Cast.h"

//@formatter:off
#if BIO_CPP_VERSION < 11
	#ifdef BIO_OS_IS_LINUX
		#include <unistd.h>
	#endif
#else
	#include <chrono>
#endif
//@formatter:on

namespace bio {

Threaded::Threaded()
	:
//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			//use m_thread default ctor
		#endif
		m_id(InvalidThreadId()),
	#else
		m_thread(NULL),
	#endif
	//@formatter:on
	m_created(false),
	m_running(false),
	m_stopRequested(false)
{

}

Threaded::~Threaded()
{
	BIO_ASSERT(!m_running);
}

bool Threaded::IsRunning()
{
	LockThread();
	bool ret = m_running;
	UnlockThread();
	return ret;
}

void Threaded::RequestStop()
{
	LockThread();
	m_stopRequested = true;
	UnlockThread();
}

/*static*/ void* Threaded::Worker(void* arg)
{
	Threaded* threaded = Cast<Threaded*>(arg);
	BIO_SANITIZE(threaded, ,
		return NULL);

	threaded->LockThread();
	threaded->m_running = true;
	threaded->UnlockThread();

	bool again = true;
	while (again)
	{
		bool again = threaded->Work();
		threaded->LockThread();
		again = again && threaded->m_stopRequested;
		threaded->UnlockThread();
	}

	threaded->LockThread();
	threaded->m_running = false;
	threaded->UnlockThread();

	return NULL;
}

Threaded::ThreadId Threaded::GetThreadId()
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		return m_id;
	#else
		BIO_SANITIZE(m_thread,,return InvalidThreadId())
		return m_thread->get_id();
	#endif
	//@formatter:on
}

bool Threaded::Start()
{
	LockThread();
	bool isStopped = !m_created && !m_running;
	UnlockThread();
	//@formatter:off
	BIO_SANITIZE(!isStopped, ,return true)
	//@formatter:on

	//@formatter:off
	#if BIO_CPP_VERSION < 11
		BIO_SANITIZE(m_running,,return false)
		#ifdef BIO_OS_IS_LINUX
			int result = pthread_create(&m_thread, NULL, Worker, this);
		#endif
		m_created = result == 0;
	#else
		BIO_SANITIZE(m_thread,,return false)
		m_thread = new std::thread(&Threaded::Worker, this);
		m_created = true
	#endif
	//@formatter:on
	return m_created;
}

bool Threaded::Stop()
{
	LockThread();
	bool isStopped = !m_created && !m_running;
	UnlockThread();

	BIO_SANITIZE(isStopped, ,
		return true)
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		m_stopRequested = true;
		#ifdef BIO_OS_IS_LINUX
            void** threadReturn;
			int result = pthread_join(m_thread, threadReturn);
		#endif
		RequestStop();
		m_created = false;
		return result == 0;
	#else
		BIO_SANITIZE(m_thread,,return true)
		RequestStop();
		m_thread->join();
		delete m_thread;
		m_thread = NULL;
		m_created = false;
		return true;
	#endif
	//@formatter:on

}

void Threaded::Sleep(TimeUS us)
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			usleep(us);
		#endif
	#else
		std::this_thread::sleep_for (std::chrono::microseconds(ms));
	#endif
	//@formatter:on
}

} //bio namespace
