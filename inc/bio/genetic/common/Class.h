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

#include "bio/genetic/macros/Macros.h"
#include "bio/molecular/common/Class.h"
#include "bio/molecular/Molecule.h"

namespace bio {
namespace genetic {

/**
 * A genetic::Class extends molecular::Class <br />
 * Class in other namespaces will grow to include more complex, templated logic. <br /> <br />
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>. <br />
 * @tparam T
 */
template < typename T >
class Class :
	public molecular::Class< T >,
	virtual public molecular::Molecule
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular,
		T)

	/**
	 * Providing just the object should not Initialize anything. <br />
	 * For default constructors of virtually inherited classes. <br />
	 * @param object
	 */
	Class(
		T* object, <br />
		physical::Perspective< StandardDimension >* perspective = NULL, <br />
		Filter filter = filter::Default())
		:
		molecular::Class< T >(
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
		 */
	Class(
		T* object, <br />
		Name name,
		physical::Perspective< StandardDimension >* perspective = NULL, <br />
		Filter filter = filter::Default())
		:
		molecular::Class< T >(
			object,
			name,
			perspective,
			filter
		)
	{

	}

	/**
	 * @param object
	 * @param id
	 * @param perspective
	 * @param filter
	 */
	Class(
		T* object, <br />
		StandardDimension id,
		physical::Perspective< StandardDimension >* perspective = NULL, <br />
		Filter filter = filter::Default())
		:
		molecular::Class< T >(
			object,
			id,
			perspective,
			filter
		)
	{

	}

	/**
	 *
	 */
	virtual ~Class()
	{

	}
};

} //genetic namespace
} //bio namespace
