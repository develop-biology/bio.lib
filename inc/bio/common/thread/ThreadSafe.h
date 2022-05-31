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

#include "bio/common/macros/Macros.h"

//@formatter:off
#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			#include <pthread.h>
		#endif
	#else
		#include <mutex>
	#endif
#endif
//@formatter:on

namespace bio {

/**
 * ThreadSafe classes are ones that can potentially be called concurrently. <br />
 * They are meant to be simple and abstract. <br />
 * The actual interface of ThreadSafe methods is c++ version and platform specific. <br />
 * Fortunately, threading was standardized in c++11; however, we do still support c++98 builds. <br />
 * At this time, ThreadSafe methods are only implemented for c++11 and on and on linux systems using c++98 and on. <br />
 * Support for c++98 on other platforms may or may not ever happen. <br />
 *
 * NOTE: if you do not need threading and don't want to waste time locking & unlocking a single thread all the time, check out Optimize.h (in bio/common), which will let you turn off threading for an extra performance boost (i.e. set BIO_THREAD_ENFORCEMENT_LEVEL to 0 to disable). <br />
 *
 * Please see SafelyAccess for an easy way to create external locks of ThreadSafe classes.
 */
class ThreadSafe
{
public:
	/**
	 *
	 */
	ThreadSafe();

	#if BIO_CPP_VERSION >= 11
	/**
	 * @param toMove
	 */
	ThreadSafe(ThreadSafe&& toMove);
	#endif

	/**
	 * @param toCopy
	 */
	ThreadSafe(const ThreadSafe& toCopy);

	#if BIO_CPP_VERSION >= 11
	/**
	 * @param toMove
	 * @return *this.
	 */
	ThreadSafe& operator=(ThreadSafe&& toMove);
	#endif

	/**
	 * @param toCopy
	 * @return *this.
	 */
	ThreadSafe& operator=(const ThreadSafe& toCopy);

	/**
	 *
	 */
	virtual ~ThreadSafe();

	/**
	 * uses const_cast to call LockThread(). <br />
	 */
	void LockThread() const;

	/**
	 * uses const_cast to call UnlockThread(). <br />
	 */
	void UnlockThread() const;

	//@formatter:off
	#if BIO_THREAD_ENFORCEMENT_LEVEL > 0
		#if BIO_CPP_VERSION < 11
			#ifdef BIO_OS_IS_LINUX
				mutable pthread_mutex_t mLock;
			#endif
		#else
			std::mutex mMutex;
			mutable ::std::unique_lock<std::mutex> mLock;
		#endif
	#endif
	//@formatter:on

	#if BIO_THREAD_ENFORCEMENT_LEVEL < 2
	mutable bool mIsLocked;
	#endif

private:
	void CommonConstructor();
};

} //bio namespace
