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

#include "bio/cellular/wave/PeakCarrierWave.h"
#include "bio/genetic/common/Class.h"
#include "bio/molecular/Vesicle.h"
#include "bio/physical/Periodic.h"

namespace bio {
namespace cellular {

/**
 * A cellular::Class extends genetic::Class <br />
 * Class in other namespaces will grow to include more complex, templated logic. <br />
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>. <br />
 * @tparam T a pointer to a class type.
 */
template < typename T >
class Class :
	public genetic::Class< T >,
	virtual public physical::Periodic,
	virtual public molecular::Vesicle
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(genetic, T)

	/**
	 * Providing just the object should not Initialize anything. <br />
	 * For default constructors of virtually inherited classes. <br />
	 * @param object
	 */
	Class(
		T* object,
		physical::Perspective< Id >* perspective = NULL,
		Filter filter = filter::Default())
		:
		genetic::Class< T >(
			object,
			perspective,
			filter
		)
	{

	}

	/**
	 * @param object
	 * @param name
	 * @param perspective
	 * @param filter
	 * @param interval
	 */
	Class(
		T* object,
		const Name& name,
		physical::Perspective< Id >* perspective = NULL,
		Filter filter = filter::Default(),
		MilliSeconds interval = GetDefaultInterval())
		:
		genetic::Class< T >(
			object,
			name,
			perspective,
			filter
		)
	{
		physical::Periodic::Initialize(interval);
	}

	/**
	 * @param object
	 * @param id
	 * @param perspective
	 * @param filter
	 * @param interval
	 */
	Class(
		T* object,
		const Id& id,
		physical::Perspective< Id >* perspective = NULL,
		Filter filter = filter::Default(),
		MilliSeconds interval = GetDefaultInterval())
		:
		genetic::Class< T >(
			object,
			id,
			perspective,
			filter
		)
	{
		physical::Periodic::Initialize(interval);
	}


	/**
	 *
	 */
	virtual ~Class()
	{

	}

	virtual Code Peak()
	{
		static PeakCarrierWave sPeakCarrierWave;
		return Attenuate(&sPeakCarrierWave);
	}

};

} //cellular namespace
} //bio namespace
