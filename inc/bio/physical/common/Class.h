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

#include "Codes.h"
#include "bio/physical/Wave.h"

namespace bio {
namespace physical {

/**
 * A physical::Class is a Wave. That is all. <br />
 * Class in other namespaces will grow to include more complex, templated logic. <br />
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>. <br />
 * @tparam T
 */
template < typename T >
class Class :
	public Wave
{
public:
	/**
	 * NOTE: you may wish to explicitly instantiate Wave in you ctor. <br />
	 * @param object
	 * @param symmetry
	 */
	Class(
		T* object, <br />
		Symmetry* symmetry = NULL <br />
	)
		:
		Wave(symmetry),
		m_object(object)
	{

	}

	/**
	 *
	 */
	virtual ~Class()
	{

	}

	/**
	* Make it so we can treat *this as the calling T. <br />
	* @return the m_object *this was created for.
	*/
	virtual operator T*()
	{
		return m_object;
	}

	/**
	 * If you have a Class object and need to convert from Wave to the furthest derived Class, use Convert. <br />
	 * @param wave
	 * @return the
	 */
	T* Convert(Wave* wave) <br />
	{
		return Cast< T* >(wave); <br />
	}


	/**
	 * Template override for Clone so we don't have to define it everywhere. <br />
	 * @return a new T (and a new *this).
	 */
	virtual Wave* Clone() const <br />
	{
		T* ret = new T(*m_object); <br />
		return Cast< Class< T >* >(ret); //2-step cast: 1st explicitly cast to *this; 2nd implicitly cast to Wave. <br />
	}

	/**
	 * Used for resolving ambiguous inheritance without the need to explicitly derive from Wave. <br />
	 * @return this
	 */
	virtual Wave* AsWave() <br />
	{
		return this;
	}

	/**
	 * Used for resolving ambiguous inheritance without the need to explicitly derive from Wave. <br />
	 * @return this
	 */
	virtual const Wave* AsWave() const <br />
	{
		return this;
	}

	/**
	 * Wave conversion. <br />
	 * When treating an object as a Wave*, always use the nearest Class, as that will allow you to access all other Waves in the object. <br />
	 * @return this.
	 */
	virtual operator Wave*()
	{
		return this;
	}

protected:
	T* m_object; <br />
};

} //physical namespace
} //bio namespace
