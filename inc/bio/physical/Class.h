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

#include "Wave.h"

namespace bio {
namespace physical {

/**
 * A physical::Class is a Wave. That is all.
 * Class in other namespaces will grow to include more complex, templated logic.
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>.
 * @tparam T
 */
template <typename T>
class Class : public Wave
{
public:
	/**
	 * NOTE: you may wish to explicitly instantiate Wave in you ctor.
	 * @param object
	 * @param symmetry
	 */
	Class(T* object, Symmetry* symmetry=NULL) :
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
	 * Template override for Clone so we don't have to define it everywhere.
	 * @return a new T (and a new *this).
	 */
	virtual T* Clone() const
	{
		return new T(*m_object);
	}

protected:
	T* m_object;
};

} //physical namespace
} //bio namespace