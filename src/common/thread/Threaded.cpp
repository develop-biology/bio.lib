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

#include "bio/common/thread/Threaded.h"
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
			//use mThread default constructor
		#endif
		mId(InvalidThreadId()),
	#else
		mThread(NULL),
	#endif
	//@formatter:on
	mCreated(false),
	mRunning(false),
	mStopRequested(false)
{

}

Threaded::~Threaded()
{
	BIO_ASSERT(!mRunning);
}

bool Threaded::IsRunning()
{
	LockThread();
	bool ret = mRunning;
	UnlockThread();
	return ret;
}

void Threaded::RequestStop()
{
	LockThread();
	mStopRequested = true;
	UnlockThread();
}

/*static*/ void* Threaded::Worker(void* arg)
{
	Threaded* threaded = Cast< Threaded* >(arg);
	BIO_SANITIZE(threaded, ,
		return NULL);

	threaded->LockThread();
	threaded->mRunning = true;
	threaded->UnlockThread();

	bool again = true;
	while (again)
	{
		bool again = threaded->Work();
		threaded->LockThread();
		again = again && threaded->mStopRequested;
		threaded->UnlockThread();
	}

	threaded->LockThread();
	threaded->mRunning = false;
	threaded->UnlockThread();

	return NULL;
}

Threaded::ThreadId Threaded::GetThreadId()
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		return mId;
	#else
		BIO_SANITIZE(mThread,,return InvalidThreadId())
		return mThread->get_id();
	#endif
	//@formatter:on
}

bool Threaded::Start()
{
	LockThread();
	bool isStopped = !mCreated && !mRunning;
	UnlockThread();
	//@formatter:off
	BIO_SANITIZE(!isStopped, ,return true)
	//@formatter:on

	//@formatter:off
	#if BIO_CPP_VERSION < 11
		BIO_SANITIZE(mRunning,,return false)
		#ifdef BIO_OS_IS_LINUX
			int result = pthread_create(&mThread, NULL, Worker, this);
		#endif
		mCreated = result == 0;
	#else
		BIO_SANITIZE(mThread,,return false)
		mThread = new ::std::thread(&Threaded::Worker, this);
		mCreated = true;
	#endif
	//@formatter:on
	return mCreated;
}

bool Threaded::Stop()
{
	LockThread();
	bool isStopped = !mCreated && !mRunning;
	UnlockThread();

	BIO_SANITIZE(isStopped, ,
		return true)
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		mStopRequested = true;
		#ifdef BIO_OS_IS_LINUX
            void** threadReturn; 
			int result = pthread_join(mThread, threadReturn);
		#endif
		RequestStop();
		mCreated = false;
		return result == 0;
	#else
		BIO_SANITIZE(mThread,,return true)
		RequestStop();
		mThread->join();
		delete mThread;
		mThread = NULL;
		mCreated = false;
		return true;
	#endif
	//@formatter:on

}

void Threaded::Sleep(MilliSeconds us)
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		#ifdef BIO_OS_IS_LINUX
			usleep(us);
		#endif
	#else
		std::this_thread::sleep_for (std::chrono::milliseconds(us));
	#endif
	//@formatter:on
}

} //bio namespace
