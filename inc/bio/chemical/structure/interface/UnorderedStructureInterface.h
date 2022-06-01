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

#include "bio/common/thread/ThreadSafe.h"
#include "bio/chemical/Atom.h"
#include "bio/chemical/structure/motif/LinearMotif.h"

namespace bio {
namespace chemical {

/**
 * Interface methods for all UnorderedMotif classes. <br />
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
	 * Adds to *this using the appropriate interface for T. <br />
	 * Does not allow duplicates. <br />
	 * @tparam T
	 * @param t
	 * @return the t inserted or 0; 0 if T is invalid.
	 */
	template < typename T >
	T Add(const T t)
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->AddImplementation(t),
			return 0
		)
	}


	/**
	 * Removes t from *this using the appropriate handler for T. <br />
	 * @tparam T
	 * @param t
	 * @return the removed content or 0; 0 if T is invalid.
	 */
	template < typename T >
	T Remove(const T t)
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->RemoveImplementation(t),
			return 0
		)
	}


	/**
	 * Copy the contents of another container into *this. <br />
	 * Does nothing if T is invalid. <br />
	 * @tparam T
	 * @param other
	 */
	template < typename T >
	void Import(const UnorderedMotif< T >* other)
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			implementer->ImportImplementation(other),
		)
	}

	/**
	 * Copy the contents of a vector into *this. <br />
	 * Will only work if *this contains an UnorderedMotif of the given type. <br />
	 * Does nothing if T is invalid. <br />
	 * @tparam T
	 * @param other
	 */
	template < typename T >
	void Import(const ::bio::Arrangement< T >& other)
	{
		for (
			SmartIterator otr = other.Begin();
			!otr.IsAfterEnd();
			++otr
			)
		{
			this->Add< T >(*otr);
		}
	}

	/**
	 * This method does way more than it should reasonably be able to. <br />
	 * Here, we take advantage of some of the Biology features that are starting to form. Primarily, we leverage physical::Properties and Bonds (per Atom) to search through the pseudo-vtable of Atom, find all StructuralComponents in *this and attempt to Import the corresponding StructuralComponents of other. <br />
	 * This method side-steps the typical inheritance encapsulation in order to prevent child classes from having to override this method and account for each new UnorderedMotif they add. In other words, complexity here removes repeated code downstream. <br />
	 * @param other
	 */
	Code ImportAll(const physical::Wave* other)
	{
		BIO_SANITIZE(other && other->AsAtom(), ,
			return code::BadArgument1())

		Code ret = code::Success();

		Bond* bondBuffer;
		for (
			SmartIterator bnd = other->AsAtom()->GetAllBonds()->End();
			!bnd.IsBeforeBeginning();
			--bnd
			)
		{
			bondBuffer = bnd;
			if (bondBuffer->IsEmpty())
			{
				continue;
			}
			if (physical::Wave::GetResonanceBetween(
				bondBuffer->GetBonded(),
				AbstractMotif::GetClassProperties()).Size() == 0)
			{
				continue;
			}
			const physical::Wave* otherBond = other->AsAtom()->GetBonded(other->AsAtom()->GetBondPosition(bondBuffer->GetId()));
			(Cast< AbstractMotif* >(bondBuffer->GetBonded()))->ImportImplementation(otherBond); //actual work 
		}
		return ret;
	}

	/**
	 * Gives the number of T in *this <br />
	 * @tparam T
	 * @return the size of contents; 0 if T is invalid.
	 */
	template < typename T >
	Index GetCount() const
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetCountImplementation(),
			return 0
		)
	}


	/**
	 * USE WITH CAUTION!!! <br />
	 * @tparam T
	 * @return A pointer to all contents in *this; 0 if T is invalid.
	 */
	template < typename T >
	Container* GetAll()
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetAllImplementation(),
			return 0
		)
	}


	/**
	 * Safer, const version of above. <br />
	 * @tparam T
	 * @return A pointer to all contents in *this; 0 if T is invalid.
	 */
	template < typename T >
	const Container* GetAll() const
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetAllImplementation(),
			return 0
		)
	}


	/**
	 * Check for content. <br />
	 * @tparam T
	 * @param content
	 * @return whether or not content exists in *this; false if T is invalid.
	 */
	template < typename T >
	bool Has(T content) const
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->HasImplementation(content),
			return false
		)
	}

	/**
	 * Gives the number of matching contents between *this & other. <br />
	 * @param other
	 * @return quantity overlap with other; 0 if T is invalid.
	 */
	template < typename T >
	unsigned int GetNumMatching(const Container* other) const
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetNumMatchingImplementation(other),
			return 0
		)
	}

	/**
	 * Check if *this contains all of the given contents <br />
	 * Should NOT check if the given contents contain all those of *this. <br />
	 * @param content
	 * @return whether or not the given contents exists in *this
	 */
	template < typename T >
	bool HasAll(const Container* contents) const
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->HasAllImplementation(contents),
			return false
		)
	}

	/**
	 * Removes all T from *this. <br />
	 * Does not delete the contents! <br />
	 * Does nothing if T is invalid. <br />
	 * @tparam T
	 */
	template < typename T >
	void Clear()
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			implementer->ClearImplementation(),
		)
	}

	/**
	 * Get the Contents of *this as a string. <br />
	 * @param separator e.g. ", ", the default, or just " ".
	 * @return the Contents of *this as a string; "" if T is invalid.
	 */
	template < typename T >
	std::string GetStringFrom(std::string separator = ", ")
	{
		UnorderedMotif< T >* implementer = this->AsBonded< UnorderedMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetStringFromImplementation(separator),
			return "");
	}

	/**
	 * Ease of use wrapper around casting the contents of *this as a ::bio::Arrangement. <br />
	 * @tparam T
	 * @return the contents of *this casted to an ::bio::Arrangement.
	 */
	template < typename T >
	std::vector< T > GetAllAsVector()
	{
		return this->template GetAll< T >()->
			template AsVector< T >();
	}

	/**
	 * Ease of use wrapper around casting the contents of *this as a ::bio::Arrangement. <br />
	 * @tparam T
	 * @return the contents of *this casted to an ::bio::Arrangement.
	 */
	template < typename T >
	const ::std::vector< T > GetAllAsVector() const
	{
		return this->template GetAll< T >()->
			template AsVector< T >();
	}
};

} //chemical namespace
} //bio namespace
