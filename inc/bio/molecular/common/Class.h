/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "bio/chemical/common/Class.h"
#include "bio/chemical/solution/Solute.h"

namespace bio {
namespace molecular {

/**
 * A molecular::Class extends chemical::Class <br />
 * Class in other namespaces will grow to include more complex, templated logic. <br />
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>. <br />
 * @tparam T a pointer to a class type.
 */
template < typename T >
class Class :
	public chemical::Class< T >,
	virtual public chemical::Substance,
	virtual public ThreadSafe
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, T)

	/**
	 * Providing just the object should not Initialize anything. <br />
	 * For default constructors of virtually inherited classes. <br />
	 * @param object
	 * @param filter
	 * @param symmetryType
	 */
	Class(
		T* object,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		chemical::Class< T >(
			object,
			filter,
			symmetryType
		)
	{

	}

	/**
	 * @param object
	 * @param name
	 * @param filter
	 * @param symmetryType
	 */
	Class(
		T* object,
		const Name& name,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		chemical::Class< T >(
			object,
			name,
			filter,
			symmetryType
		)
	{

	}

	/**
	 * @param object
	 * @param id
	 * @param filter
	 * @param symmetryType
	 */
	Class(
		T* object,
		const Id& id,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		chemical::Class< T >(
			object,
			id,
			filter,
			symmetryType
		)
	{

	}

	/**
	 *
	 */
	virtual ~Class()
	{

	}

	/**
	 * When inheriting from multiple EnvironmentDependent classes, this method can be used to easily specify which Environment to use. <br />
	 * @tparam ENVIRONMENT
	 * @return GetEnvironment from an EnvironmentDependent base.
	 */
	template < typename ENVIRONMENT >
	ENVIRONMENT GetEnvironment()
	{
		return this->physical::Class< T >::GetWaveObject()->chemical::template EnvironmentDependent< ENVIRONMENT >::GetEnvironment();
	}

	/**
	 * When inheriting from multiple EnvironmentDependent classes, this method can be used to easily specify which Environment to use. <br />
	 * @tparam ENVIRONMENT
	 * @return GetEnvironment from an EnvironmentDependent base.
	 */
	template < typename ENVIRONMENT >
	const ENVIRONMENT GetEnvironment() const
	{
		return this->physical::Class< T >::GetWaveObject()->chemical::template EnvironmentDependent< ENVIRONMENT >::GetEnvironment();
	}
};

} //molecular namespace
} //bio namespace
