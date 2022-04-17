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

#include "bio/common/ThreadSafe.h"

namespace bio {

ThreadSafe::ThreadSafe()
//@formatter:off
	#if BIO_CPP_VERSION < 11
    #else
        :
		m_mutex(),
		m_lock(m_mutex, std::defer_lock)
	#endif
	//@formatter:on
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			pthread_mutexattr_t mutexattr;
			pthread_mutexattr_init(&mutexattr);
			pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&m_lock, &mutexattr);
		#endif
	#else
	#endif
	//@formatter:on
}

ThreadSafe::ThreadSafe(const ThreadSafe& toCopy)
//@formatter:off
	#if BIO_CPP_VERSION < 11
    #else
        :
		m_mutex(),
		m_lock(m_mutex, std::defer_lock)
	#endif
	//@formatter:on
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			pthread_mutexattr_t mutexattr;
			pthread_mutexattr_init(&mutexattr);
			pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&m_lock, &mutexattr);
		#endif
	#else
	#endif
	//@formatter:on
}

ThreadSafe::~ThreadSafe()
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			pthread_mutex_destroy(&m_lock);
		#endif
	#else
	#endif
	//@formatter:on
}

void ThreadSafe::LockThread() const
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			pthread_mutex_lock(&m_lock);
		#endif
	#else
		m_lock.lock();
	#endif
	//@formatter:on
}

void ThreadSafe::UnlockThread() const
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			pthread_mutex_unlock(&m_lock);
		#endif
	#else
		m_lock.unlock();
	#endif
	//@formatter:on
}

} //bio namespaace
