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

#include "bio/common/thread/ThreadSafe.h"

namespace bio {

void ThreadSafe::CommonConstructor()
{
	//@formatter:off
	#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
		#if BIO_CPP_VERSION < 11
	#ifdef BIO_OS_IS_LINUX
				pthread_mutexattr_t mutexattr;
				pthread_mutexattr_init(&mutexattr);
				pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);
				pthread_mutex_init(&mLock, &mutexattr);
			#endif
		#else
		#endif
	#endif
	//@formatter:on

	#if BIO_THREAD_ENFORCEMENT_LEVEL < 2
	mIsLocked = false;
	#endif
}

ThreadSafe::ThreadSafe()
	//@formatter:off
	#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
		#if BIO_CPP_VERSION < 11
		#else
			:
			mMutex(),
			mLock(mMutex, ::std::defer_lock)
		#endif
	#endif
	//@formatter:on
{
	CommonConstructor();
}

#if BIO_CPP_VERSION >= 11
ThreadSafe::ThreadSafe(ThreadSafe&& toMove)
	//@formatter:off
	#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
		#if BIO_CPP_VERSION < 11
		#else
			:
			mMutex(),
			mLock(mMutex, ::std::defer_lock)
		#endif
	#endif
	//@formatter:on
{
	CommonConstructor();
}
#endif

ThreadSafe::ThreadSafe(const ThreadSafe& toCopy)
//@formatter:off
#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
	#if BIO_CPP_VERSION < 11
	#else
	:
	mMutex(),
	mLock(mMutex, ::std::defer_lock)
#endif
#endif
//@formatter:on
{
	CommonConstructor();
}

ThreadSafe::~ThreadSafe()
{
	//@formatter:off
	#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
		#if BIO_CPP_VERSION < 11
			#ifdef BIO_OS_IS_LINUX
				pthread_mutex_destroy(&mLock);
			#endif
		#else
		#endif
	#endif
	//@formatter:on
}

#if BIO_CPP_VERSION >= 11
ThreadSafe& ThreadSafe::operator=(ThreadSafe&& toMove)
{
	//mutexes have already been created by the time assignment can be called.
	return *this;
}
#endif

ThreadSafe& ThreadSafe::operator=(const ThreadSafe& toCopy)
{
	//mutexes have already been created by the time assignment can be called.
	return *this;
}

void ThreadSafe::LockThread() const
{
	#if BIO_THREAD_ENFORCEMENT_LEVEL < 2
	BIO_SANITIZE(!mIsLocked,,return)
	mIsLocked = true;
	#endif

	//@formatter:off
	#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
		#if BIO_CPP_VERSION < 11
			#ifdef BIO_OS_IS_LINUX
				pthread_mutex_lock(&mLock);
			#endif
		#else
			mLock.lock();
		#endif
	#endif
	//@formatter:on
}

void ThreadSafe::UnlockThread() const
{
	#if BIO_THREAD_ENFORCEMENT_LEVEL < 2
	BIO_SANITIZE(mIsLocked,,return)
	mIsLocked = false;
	#endif

	//@formatter:off
	#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
		#if BIO_CPP_VERSION < 11
			#ifdef BIO_OS_IS_LINUX
				pthread_mutex_unlock(&mLock);
			#endif
		#else
			mLock.unlock();
		#endif
	#endif
	//@formatter:on
}

} //bio namespace
