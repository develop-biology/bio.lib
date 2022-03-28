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

#include "LinearStructuralComponentImplementation.h"
#include "StructureInterface.h"

namespace bio {
namespace chemical {

class ExcitationBase;

/**
 * Interface methods for all LinearStructuralComponent classes.
 */
class LinearStructureInterface :
	virtual public StructureInterface
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
	 * Inserts toAdd into *this at then indicated position.
	 * Removes any conflicting Contents of the same Id as toAdd.
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
		const typename LinearStructuralComponentImplementation< T >::Dimension optionalPositionArg = 0, // assuming 0 is the InvalidId for all Perspectives.
		const bool transferSubContents = false
	)
	{
		Code ret = code::GeneralFailure();
		LockThread();
		LinearStructuralComponentImplementation< T >* implementer = this->AsBonded< LinearStructuralComponentImplementation< T >* >();
		if (implementer)
		{
			ret = implementer->InsertImplementation(
				toAdd,
				position,
				optionalPositionArg,
				transferSubContents
			);
		}
		UnlockThread();
		return ret;
	}

	/**
	 * Gets a T* by its id.
	 * @tparam T
	 * @param id
	 * @return a T of the given id or NULL; NULL if T is invalid.
	 */
	template < typename T >
	T GetById(
		typename LinearStructuralComponentImplementation< T >::Dimension id
	)
	{
		T ret = NULL;
		LockThread();
		LinearStructuralComponentImplementation< T >* implementer = this->AsBonded< LinearStructuralComponentImplementation< T >* >();
		if (implementer)
		{
			ret = implementer->GetByIdImplementation(
				id
			);
		}
		UnlockThread();
		return ret;
	}

	/**
	 * Gets a const T* by its id.
	 * @tparam T
	 * @param id
	 * @return a T of the given id or NULL; NULL if T is invalid.
	 */
	template < typename T >
	const T GetById(
		typename LinearStructuralComponentImplementation< T >::Dimension id
	) const
	{
		T ret = NULL;
		LockThread();
		LinearStructuralComponentImplementation< T >* implementer = this->AsBonded< LinearStructuralComponentImplementation< T >* >();
		if (implementer)
		{
			ret = implementer->GetByIdImplementation(
				id
			);
		}
		UnlockThread();
		return ret;
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
		T ret = NULL;
		LockThread();
		LinearStructuralComponentImplementation< T >* implementer = this->AsBonded< LinearStructuralComponentImplementation< T >* >();
		if (implementer)
		{
			ret = implementer->GetByNameImplementation(
				name
			);
		}
		UnlockThread();
		return ret;
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
		T ret = NULL;
		LockThread();
		LinearStructuralComponentImplementation< T >* implementer = this->AsBonded< LinearStructuralComponentImplementation< T >* >();
		if (implementer)
		{
			ret = implementer->GetByNameImplementation(
				name
			);
		}
		UnlockThread();
		return ret;
	}

	/**
	 * Tries to find a Content of the given id in *this and, optionally, the Contents beneath.
	 * If such an object doesn't exist, one is created from its Wave.
	 * @tparam T
	 * @param id
	 * @return A T* of the given id; NULL if T is invalid.
	 */
	template < typename T >
	T GetOrCreateById(
		typename LinearStructuralComponentImplementation< T >::Dimension id
	)
	{
		T ret = NULL;
		LockThread();
		LinearStructuralComponentImplementation< T >* implementer = this->AsBonded< LinearStructuralComponentImplementation< T >* >();
		if (implementer)
		{
			ret = implementer->GetOrCreateByIdImplementation(
				id
			);
		}
		UnlockThread();
		return ret;
	}

	/**
	 * Tries to find a Content of the given name in *this and, optionally, the Contents beneath.
	 * If such an object doesn't exist, one is created from its Wave.
	 * @tparam T
	 * @param name
	 * @return A T* of the given id; NULL if T is invalid.
	 */
	template < typename T >
	T GetOrCreateByName(
		Name name
	)
	{
		T ret = NULL;
		LockThread();
		LinearStructuralComponentImplementation< T >* implementer = this->AsBonded< LinearStructuralComponentImplementation< T >* >();
		if (implementer)
		{
			ret = implementer->GetOrCreateByNameImplementation(
				name
			);
		}
		UnlockThread();
		return ret;
	}


	/**
	 * Performs the given Reaction on all contents.
	 * @param reaction
	 * @param self a pointer to *this, if *this is a chemical::Substance.
	 */
	template < typename T >
	Emission ForEach(
		ExcitationBase* excitation
	)
	{
		Emission ret;
		LockThread();
		LinearStructuralComponentImplementation< T >* implementer = this->AsBonded< LinearStructuralComponentImplementation< T >* >();
		if (implementer)
		{
			ret = implementer->ForEachImplementation(excitation);
		}
		UnlockThread();
		return ret;
	}
};

} //chemical namespace
} //bio namespace
