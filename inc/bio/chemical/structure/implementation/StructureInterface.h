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

#pragma once

#include "StructuralComponentImplementation.h"
#include "bio/common/ThreadSafe.h"
#include "bio/chemical/Atom.h"

namespace bio {
namespace chemical {

/**
 * Interface methods for all StructuralComponent classes.
 */
class StructureInterface :
	virtual public ThreadSafe,
	virtual public Atom
{
public:

	/**
	 *
	 */
	StructureInterface()
	{
	}

	/**
	 *
	 */
	virtual ~StructureInterface()
	{
	}

	/**
	 * Gets a pointer to the t within *this using the appropriate implementation for T.
	 * @tparam T
	 * @param t
	 * @return a T* pointing to the contents of *this or NULL; NULL if T is invalid.
	 */
	template < typename T >
	T* Get(T t)
	{
		T* ret = NULL;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->GetImplementation(t),);
		UnlockThread();
		return ret;
	}

	/**
	 * Gets a pointer to the t within *this using the appropriate implementation for T.
	 * @tparam T
	 * @param t
	 * @return a const T* pointing to the contents of *this or NULL; NULL if T is invalid.
	 */
	template < typename T >
	const T* Get(T t) const
	{
		const T* ret = NULL;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->GetImplementation(t),);
		UnlockThread();
		return ret;
	}


	/**
	 * Adds to *this using the appropriate implementation for T.
	 * Does not allow duplicates.
	 * @tparam T
	 * @param t
	 * @return the t inserted or NULL; NULL if T is invalid.
	 */
	template < typename T >
	T* Add(T t)
	{
		T* ret = NULL;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->AddImplementation(t),);
		UnlockThread();
		return ret;
	}


	/**
	 * Removes t from *this using the appropriate handler for T.
	 * @tparam T
	 * @param t
	 * @return the removed content or NULL; NULL if T is invalid.
	 */
	template < typename T >
	T* Remove(T t)
	{
		T* ret = NULL;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->RemoveImplementation(t),);
		UnlockThread();
		return ret;
	}


	/**
	 * Copy the contents of another container into *this.
	 * Does nothing if T is invalid.
	 * @tparam T
	 * @param other
	 */
	template < typename T >
	void Import(const StructuralComponentImplementation <T>* other)
	{
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			(Cast< StructuralComponentImplementation< T >* >(RESULT))->ImportImplementation(other),);
		UnlockThread();
	}

	/**
	 * Gives the number of T in *this
	 * @tparam T
	 * @return the size of contents; 0 if T is invalid.
	 */
	template < typename T >
	unsigned long GetCount() const
	{
		unsigned long ret = 0;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->GetCountImplementation(),);
		UnlockThread();
		return ret;
	}


	/**
	 * USE WITH CAUTION!!!
	 * @tparam T
	 * @return A pointer to all contents in *this; NULL if T is invalid.
	 */
	template < typename T >
	typename StructuralComponentImplementation< T >::Contents* GetAll()
	{
		typename StructuralComponentImplementation< T >::Contents* ret = NULL;
		LockThread();
		BIO_SANITIZE_WITH_CACHE((StructuralComponentImplementation< T >*)(this->AsBonded< StructuralComponentImplementation< T > >()),
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->GetAllImplementation(),);
		UnlockThread();
		return ret;
	}


	/**
	 * Safer, const version of above.
	 * @tparam T
	 * @return A pointer to all contents in *this; NULL if T is invalid.
	 */
	template < typename T >
	const typename StructuralComponentImplementation< T >::Contents* GetAll() const
	{
		const typename StructuralComponentImplementation< T >::Contents* ret = NULL;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->GetAllImplementation(),);
		UnlockThread();
		return ret;
	}


	/**
	 * Check for content.
	 * @tparam T
	 * @param content
	 * @return whether or not content exists in *this; false if T is invalid.
	 */
	template < typename T >
	bool Has(T t) const
	{
		bool ret = false;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->HasImplementation(t),);
		UnlockThread();
		return ret;
	}

	/**
	 * Gives the number of matching contents between *this & other.
	 * @param other
	 * @return quantity overlap with other; 0 if T is invalid.
	 */
	template < typename T >
	unsigned int GetNumMatching(const StructuralComponentImplementation <T>& other) const
	{
		unsigned int ret = 0;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->GetNumMatchingImplementation(other),);
		UnlockThread();
		return ret;
	}

	/**
	 * Check for all contents
	 * @param content
	 * @return whether or not the given contents exists in *this
	 */
	template < typename T >
	bool HasAll(const typename StructuralComponentImplementation< T >::Contents& contents)
	{
		bool ret = false;
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->HasAllImplementation(contents),);
		UnlockThread();
		return ret;
	}

	/**
	 * Removes all T from *this.
	 * Does not delete the contents!
	 * Does nothing if T is invalid.
	 * @tparam T
	 */
	template < typename T >
	void Clear()
	{
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			return (Cast< StructuralComponentImplementation< T >* >(RESULT))->ClearImplementation(),);
		UnlockThread();
	}

	/**
	 * Get the Contents of *this as a string.
	 * @param separator e.g. ", ", the default, or just " ".
	 * @return the Contents of *this as a string; "" if T is invalid.
	 */
	template < typename T >
	std::string GetStringFrom(std::string separator = ", ")
	{
		std::string ret = "";
		LockThread();
		BIO_SANITIZE_WITH_CACHE(this->AsBonded < StructuralComponentImplementation< T > >,
			ret = (Cast< StructuralComponentImplementation< T >* >(RESULT))->GetStringFromImplementation(separator),);
		UnlockThread();
		return ret;
	}
};

} //chemical namespace
} //bio namespace
