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

#include "bio/chemical/structure/implementation/StructuralComponentImplementation.h"
#include "bio/chemical/structure/implementation/StructureInterface.h"
#include "bio/chemical/Class.h"
#include "bio/chemical/Macros.h"

namespace bio {
namespace chemical {

/**
 * StructuralComponent classes have Content classes stored within them.
 * They are simple containers.
 */
template < typename CONTENT_TYPE >
class StructuralComponent :
	virtual public StructureInterface,
	public Class< StructuralComponent< CONTENT_TYPE > >,
	public StructuralComponentImplementation< CONTENT_TYPE >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(chemical, StructuralComponent< CONTENT_TYPE >)

	/**
	 *
	 */
	StructuralComponent()
		:
		Class< StructuralComponent< CONTENT_TYPE > >(this)
	{
	}

	/**
	 * @param contents
	 */
	explicit StructuralComponent(typename StructuralComponentImplementation< CONTENT_TYPE >::Contents contents)
		:
		Class< StructuralComponent< CONTENT_TYPE > >(this),
		StructuralComponentImplementation< CONTENT_TYPE >(contents)
	{
	}

	/**
	 *
	 */
	virtual ~StructuralComponent()
	{
	}

	/**
	 * @param toCopy
	 */
	StructuralComponent(const StructuralComponent< CONTENT_TYPE >& toCopy)
	{
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator cnt = toCopy.m_contents.begin();
			cnt != toCopy.m_contents.end();
			++cnt
			)
		{
			this->m_contents.push_back(new CONTENT_TYPE(**cnt));
		}
	}

	#if 0
	//We are only using physical::Identifiable<>::operator== for Id comparison at the moment; contents do not matter (i.e. the performance cost is too high).

	/**
	 * @param other
	 * @return whether or not the contents of *this match the contents of other.
	 */
	virtual bool operator==(const StructuralComponent<CONTENT_TYPE>& other) const
	{
		for (
			typename StructuralComponentImplementation<CONTENT_TYPE>::Contents::const_iterator cnt = other.m_contents.begin();
			cnt != other.m_contents.end();
			++cnt
			)
		{
			this->HasImplementation(*cnt);
		}
	}
	#endif

};

} //chemical namespace
} //bio namespace
