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

#include <assert.h>
#include "bio/common/macros/Macros.h"

/**
 * SafelyAccess<> is a simple RAII wrapper around a ThreadSafe object. <br />
 * If you don't supply the object to wrap, we assume it's a bio::Singleton with an Instance() method. <br />
 * To access the members of a SafelyAccess<>ed class, please use the -> operator and only dereference if you absolutely must. <br />
 * As always when dealing with threads, be careful about where you create and destroy these. Don't keep them around too long and don't deadlock ;) <br />
 * @tparam CLASS a child of bio::ThreadSafe.
 */
template < class CLASS >
class SafelyAccess
{
public:
	/**
	 * Constructor for Singletons. <br />
	 */
	SafelyAccess() :
		mClass(&CLASS::Instance())
	{
		this->CtorCommon();
	}

	/**
	 * Constructor for all other ThreadSafe objects. <br />
	 * @param toAccess
	 */
	SafelyAccess(CLASS* toAccess) :
		mClass(toAccess)
	{
		this->CtorCommon();
	}

	/**
	 * Release RAII lock. <br />
	 */
	~SafelyAccess()
	{
		this->mClass->UnlockThread();
	}

	/**
	 * Access the locked object. <br />
	 * @return a pointer to the locked CLASS.
	 */
	CLASS* operator->()
	{
		return this->mClass;
	}

	/**
	 * Access the locked object. <br />
	 * @return a pointer to the locked CLASS.
	 */
	const CLASS* operator->() const
	{
		return this->mClass;
	}

	/**
	 * Access the locked object. <br />
	 * @return a pointer to the locked CLASS.
	 */
	CLASS* operator*()
	{
		return mClass;
	}

	/**
	 * Access the locked object. <br />
	 * @return a pointer to the locked CLASS.
	 */
	const CLASS* operator*() const
	{
		return mClass;
	}

private:
	CLASS* mClass;

	void CtorCommon()
	{
		BIO_ASSERT(this->mClass)
		this->mClass->LockThread();
	}

	//remove copy ctors.
	SafelyAccess(SafelyAccess const &);
    void operator=(SafelyAccess const &);
};
