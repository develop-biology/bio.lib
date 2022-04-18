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

#include "Perspective.h"
#include "bio/common/VirtualBase.h"
#include "bio/common/macros/Macros.h"

namespace bio {
namespace physical {

/**
 * An Observer class is one that has a PERSPECTIVE. <br />
 * This is intended for Identifiable classes (see Identifiable.h) and any other classes that manage or work with Identifiable classes. <br />
 *
 * NOTE: At this time we do not currently support viewing objects from multiple Perspectives. This is because if you can get the object's Id, you should also be able to get it's Perspective (i.e observer->GetPerspective). <br />
 * By not allowing objects to be duplicated across perspectives, we reduce our overall memory footprint. <br />
*/
template < typename PERSPECTIVE >
class Observer :
	protected VirtualBase
{
public:
	typedef PERSPECTIVE Perspective;

	/**
	 * @param perspective
	 */
	explicit Observer(Perspective* perspective = NULL) 
		:
		m_perspective(perspective)
	{
	}

	/**
	 * @param other
	 */
	Observer(const Observer& other)
		:
		m_perspective(other.m_perspective)
	{
	}

	/**
	 *
	 */
	virtual ~Observer()
	{
	}

	/**
	 * Sets the perspective for *this. <br />
	 * @param perspective
	 */
	virtual void SetPerspective(Perspective* perspective) 
	{
		m_perspective = perspective;
	}

	virtual /**
	 * @return the perspective held by *this.
	 */
	Perspective* GetPerspective() const 
	{
		return m_perspective;
	}

protected:
	/**
	 * VirtualBase required method. See that class for details (in common/) <br />
	 * @param args
	 */
	virtual void InitializeImplementation(ByteStreams args)
	{
		BIO_SANITIZE(args.size() == 1 && args[0].Is(m_perspective), ,
			return);
		m_perspective = args[0];
	}

private:
	Perspective* m_perspective; 
};

} //physical namespace
} //bio namespace
