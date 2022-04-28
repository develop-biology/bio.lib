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

#include "bio/physical/Perspective.h"
#include "bio/common/cache/Cached.h"

namespace bio {

/**
 * CachedId<> extends the Cache system by making it possible to store the Perspective* from which to fetch the Id of the given Name. <br />
 * @tparam ID_TYPE 
 */
template < typename ID_TYPE >
class CachedId :
	public Cached< ID_TYPE, Name, ID_TYPE (physical::Perspective< ID_TYPE >::*)(Name) >
{
public:

	/**
	 * @param lookup
	 * @param perspective
	 */
	CachedId(
		Name lookup,
		physical::Perspective< ID_TYPE >& perspective
	)
		:
		Cached< ID_TYPE, Name, ID_TYPE (physical::Perspective< ID_TYPE >::*)(Name) >(
			lookup,
			0,
			&physical::Perspective< ID_TYPE >::GetIdFromName
		),
		m_perspective(perspective)
	{

	}

	/**
	 *
	 */
	virtual ~CachedId()
	{

	}

	/**
	 * Remove whatever *this has cached and re-look up the newest value.
	 */
	virtual void Flush()
	{
		this->m_t = (this->m_perspective.*(this->m_LookupFunction))(this->m_lookup);
	}

	/**
	 * For full transparency, this method must be added to all children. <br />
	 * (We don't have Class<> logic to save us from this nightmare at the common/ level). <br />
	 * @param out
	 * @param t
	 * @return out
	 */
	friend ::std::ostream& operator<<(
		std::ostream& out,
		const CachedId& t
	)
	{
		out << t.m_t;
		return out;
	}

protected:
	physical::Perspective< ID_TYPE >& m_perspective;
};

} //bio namespace
