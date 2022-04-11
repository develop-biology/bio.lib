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

#include "bio/common/ThreadSafe.h"
#include "bio/chemical/Atom.h"
#include "bio/chemical/structure/motif/LinearMotif.h"

namespace bio {
namespace chemical {

/**
 * Interface methods for all UnorderedMotif classes.
 */
class UnorderedStructureInterface :
	virtual public ThreadSafe,
	virtual public Atom
{
public:

	/**
	 *
	 */
	UnorderedStructureInterface()
	{
	}

	/**
	 *
	 */
	virtual ~UnorderedStructureInterface()
	{
	}


	/**
	 * Adds to *this using the appropriate interface for T.
	 * Does not allow duplicates.
	 * @tparam T
	 * @param t
	 * @return the t inserted or 0; 0 if T is invalid.
	 */
	template < typename T >
	T Add(const T t)
	{
		T ret = 0;
		LockThread();
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->AddImplementation(t);
		}
		UnlockThread();
		return ret;
	}


	/**
	 * Removes t from *this using the appropriate handler for T.
	 * @tparam T
	 * @param t
	 * @return the removed content or 0; 0 if T is invalid.
	 */
	template < typename T >
	T Remove(const T t)
	{
		T ret = 0;
		LockThread();
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->RemoveImplementation(t);
		}
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
	void Import(const UnorderedMotif <T>* other)
	{
		LockThread();
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			implementer->ImportImplementation(other);
		}
		UnlockThread();
	}

	/**
	 * Copy the contents of a vector into *this.
	 * Will only work if *this contains a UnorderedMotif of the given type.
	 * Does nothing if T is invalid.
	 * @tparam T
	 * @param other
	 */
	template < typename T >
	void Import(const std::vector< T >& other)
	{
		UnorderedMotif<T> buffer(other);
		this->Import<T>(&buffer);
	}

	/**
	 * This method does way more than it should reasonably be able to.
	 * Here, we take advantage of some of the Biology features that are starting to form. Primarily, we leverage physical::Properties and Bonds (per Atom) to search through the pseudo-vtable of Atom, find all StructuralComponents in *this and attempt to Import the corresponding StructuralComponents of other.
	 * This method side-steps the typical inheritance encapsulation in order to prevent child classes from having to override this method and account for each new UnorderedMotif they add. In other words, complexity here removes repeated code downstream.
	 * @param other
	 */
	Code ImportAll(const physical::Wave* other)
	{
		BIO_SANITIZE(other && other->AsAtom(),,return code::BadArgument1())

		Code ret = code::Success();

		LockThread(); // in case m_bonds changes.
		Bond* bondBuffer;
		for (
			SmartIterator bnd = other->AsAtom()->GetAllBonds()->End();
			!bnd.IsAtBeginning();
			--bnd
			)
		{
			*bondBuffer = bnd;
			if (bondBuffer->IsEmpty())
			{
				continue;
			}
			if (physical::Wave::GetResonanceBetween(
				bondBuffer->GetBonded(),
				AbstractMotif::GetClassProperties()).size() == 0)
			{
				continue;
			}
			const physical::Wave* otherBond = other->AsAtom()->GetBonded(other->AsAtom()->GetBondPosition(bondBuffer->GetId()));
			(Cast< AbstractMotif* >(bondBuffer->GetBonded()))->ImportImplementation(otherBond); //actual work
		}
		UnlockThread();

		return ret;
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
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->GetCountImplementation();
		}
		UnlockThread();
		return ret;
	}


	/**
	 * USE WITH CAUTION!!!
	 * @tparam T
	 * @return A pointer to all contents in *this; 0 if T is invalid.
	 */
	template < typename T >
	Container* GetAll()
	{
		Container* ret = 0;
		LockThread();
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->GetAllImplementation();
		}
		UnlockThread();
		return ret;
	}


	/**
	 * Safer, const version of above.
	 * @tparam T
	 * @return A pointer to all contents in *this; 0 if T is invalid.
	 */
	template < typename T >
	const typename UnorderedMotif< T >::Contents* GetAll() const
	{
		const typename UnorderedMotif< T >::Contents* ret = 0;
		LockThread();
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->GetAllImplementation();
		}
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
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->HasImplementation(t);
		}
		UnlockThread();
		return ret;
	}

	/**
	 * Gives the number of matching contents between *this & other.
	 * @param other
	 * @return quantity overlap with other; 0 if T is invalid.
	 */
	template < typename T >
	unsigned int GetNumMatching(const Container* other) const
	{
		unsigned int ret = 0;
		LockThread();
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->GetNumMatchingImplementation(other);
		}
		UnlockThread();
		return ret;
	}

	/**
	 * Check if *this contains all of the given contents
	 * Should NOT check if the given contents contain all those of *this.
	 * @param content
	 * @return whether or not the given contents exists in *this
	 */
	template < typename T >
	bool HasAll(const Container* contents) const
	{
		bool ret = false;
		LockThread();
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->HasAllImplementation(contents);
		}
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
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			implementer->ClearImplementation();
		}
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
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		if (implementer)
		{
			ret = implementer->GetStringFromImplementation(separator);
		}
		UnlockThread();
		return ret;
	}
};

} //chemical namespace
} //bio namespace
