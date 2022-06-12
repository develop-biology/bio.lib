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
	 * If you have a Class object and need to convert from Wave to the furthest derived Class, use Convert. <br />
	 * @param wave
	 * @return the
	 */
	static T* Convert(Wave* wave)
	{
		return Cast< T* >(wave);
	}
	
	/**
	 * NOTE: you may wish to explicitly instantiate Wave in you constructor. <br />
	 * @param object
	 * @param symmetry
	 */
	Class(
		T* object,
		Symmetry* symmetry = NULL
	)
		:
		Wave(symmetry),
		mObject(object)
	{

	}

	/**
	 *
	 */
	virtual ~Class()
	{

	}
	
	/**
	 * Simple getter.
	 * @return mObject.
	 */
	virtual T* GetWaveObject()
	{
		return mObject;
	}

	/**
	 * Simple getter.
	 * @return mObject.
	 */
	virtual const T* GetWaveObject() const
	{
		return mObject;
	}
	
	/**
	 * Make it so we can treat *this as the calling T*. <br />
	 * @return the mObject *this was created for.
	 */
	virtual operator T&()
	{
		BIO_SANITIZE(mObject, , return 0)
		return *GetWaveObject();
	}

	/**
	 * Make it so we can treat *this as the calling T*. <br />
	 * @return the mObject *this was created for.
	 */
	virtual operator const T&() const
	{
		BIO_SANITIZE(mObject, , return 0)
		return *GetWaveObject();
	}
	
	/**
	 * Template override for Clone so we don't have to define it everywhere. <br />
	 * @return a new T* (and a new *this).
	 */
	virtual Wave* Clone() const
	{
		T* ret = new T(*mObject);
		return Cast< Class< T >* >(ret)->AsWave();
	}

	/**
	 * Used for resolving ambiguous inheritance without the need to explicitly derive from Wave. <br />
	 * This intentionally hides the Wave method. <br />
	 * @return this
	 */
	Wave* AsWave()
	{
		return this;
	}

	/**
	 * Used for resolving ambiguous inheritance without the need to explicitly derive from Wave. <br />
	 * This intentionally hides the Wave method. <br />
	 * @return this
	 */
	const Wave* AsWave() const
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
	mutable T* mObject;
};

} //physical namespace
} //bio namespace
