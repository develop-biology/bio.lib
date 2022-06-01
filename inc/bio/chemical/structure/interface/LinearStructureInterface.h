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

#include "bio/chemical/structure/Structure.h"

namespace bio {
namespace chemical {

class ExcitationBase;

/**
 * Interface methods for all LinearMotif classes. <br />
 */
class LinearStructureInterface :
	virtual public ThreadSafe,
	virtual public Atom
{
public:
	/**
	 *
	 */
	LinearStructureInterface()
	{
	}

	/**
	 *
	 */
	virtual ~LinearStructureInterface()
	{
	}

	/**
	 * Inserts toAdd into *this at then indicated position. <br />
	 * Removes any conflicting Contents of the same Id as toAdd. <br />
	 * @tparam T the type of Content
	 * @param toAdd what to add.
	 * @param position where toAdd is inserted (e.g. the TOP or BOTTOM).
	 * @param optionalPositionArg If a position is specified, the optionalPositionArg is the id of the Content referenced (e.g. BEFORE, MyContentId()).
	 * @param transferSubContents allows all of the Contents within a Content that conflicts with toAdd to be copied into toAdd, before the conflicting Content is deleted (similar to renaming an upper directory while preserving it's contents).
	 */
	template < typename T >
	Code Insert(
		T toAdd,
		const Position position = BOTTOM,
		const Id optionalPositionArg = 0, // assuming 0 is the InvalidId for all Perspectives.
		const bool transferSubContents = false
	)
	{
		LinearMotif< T >* implementer = this->AsBonded< LinearMotif< T >* >();
		BIO_SANITIZE(implementer,
			BIO_SINGLE_ARG(return implementer->InsertImplementation(
				toAdd,
				position,
				optionalPositionArg,
				transferSubContents
			)),
			return code::GeneralFailure()
		)
	}

	/**
	 * Gets a T* by its id. 
	 * @tparam T
	 * @param id
	 * @return a T of the given id or NULL; NULL if T is invalid.
	 */
	template < typename T >
	T GetById(Id id)
	{
		LinearMotif< T >* implementer = this->AsBonded< LinearMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetByIdImplementation(id),
			return NULL
		)
	}

	/**
	 * Gets a const T* by its id. 
	 * @tparam T
	 * @param id
	 * @return a T of the given id or NULL; NULL if T is invalid.
	 */
	template < typename T >
	const T GetById(
		Id id
	) const
	{
		LinearMotif< T >* implementer = this->AsBonded< LinearMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetByIdImplementation(id),
			return NULL
		)
	}

	/**
	 * Gets a T* by its name. 
	 * @tparam T
	 * @param name
	 * @return a T of the given id or NULL; NULL if T is invalid.
	 */
	template < typename T >
	T GetByName(
		Name name
	)
	{
		LinearMotif< T >* implementer = this->AsBonded< LinearMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetByNameImplementation(name),
			return NULL
		)
	}

	/**
	 * Gets a const T* by its name. 
	 * @tparam T
	 * @param name
	 * @return a T of the given id or NULL; NULL if T is invalid.
	 */
	template < typename T >
	const T GetByName(
		Name name
	) const
	{
		LinearMotif< T >* implementer = this->AsBonded< LinearMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetByNameImplementation(name),
			return NULL
		)
	}

	/**
	 * Tries to find a Content of the given id in *this and, optionally, the Contents beneath. <br />
	 * If such an object doesn't exist, one is created from its Wave. <br />
	 * @tparam T
	 * @param id <br />
	 * @return A T* of the given id; NULL if T is invalid.
	 */
	template < typename T >
	T GetOrCreateById(
		Id id
	)
	{
		LinearMotif< T >* implementer = this->AsBonded< LinearMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetOrCreateByIdImplementation(id),
			return NULL
		)
	}

	/**
	 * Tries to find a Content of the given name in *this and, optionally, the Contents beneath. <br />
	 * If such an object doesn't exist, one is created from its Wave. <br />
	 * @tparam T
	 * @param name
	 * @return A T* of the given id; NULL if T is invalid.
	 */
	template < typename T >
	T GetOrCreateByName(
		Name name
	)
	{
		LinearMotif< T >* implementer = this->AsBonded< LinearMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->GetOrCreateByNameImplementation(name),
			return NULL
		)
	}


	/**
	 * Performs the given Reaction on all contents. <br />
	 * @param excitation
	 */
	template < typename T >
	Emission ForEach(ExcitationBase* excitation)
	{
		LinearMotif< T >* implementer = this->AsBonded< LinearMotif< T >* >();
		BIO_SANITIZE(implementer,
			return implementer->ForEachImplementation(excitation),
			return Emission()
		)
	}
};

} //chemical namespace
} //bio namespace
