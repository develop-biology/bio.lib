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

#include "AbstractCached.h"
#include "bio/common/TransparentWrapper.h"

namespace bio {

/**
 * Cached<> objects store some kind of type along with a trivial lookup system. <br />
 * These objects must be provided a lookup function as well as what to lookup. <br />
 * @tparam STORE_TYPE 
 */
template < typename STORE_TYPE, typename LOOKUP_TYPE >
class Cached : public AbstractCached, public TransparentWrapper< STORE_TYPE >
{
public:
	
	typedef STORE_TYPE (*LookupFunction)(LOOKUP_TYPE);

	/**
	 * @param lookup
	 * @param invalidValue
	 * @param LookupFunction
	 */
	Cached(LOOKUP_TYPE lookup, STORE_TYPE invalidValue, LookupFunction LookupFunction) :
		TransparentWrapper< STORE_TYPE >(invalidValue),
		m_lookup(lookup),
		m_LookupFunction(LookupFunction)
	{
		
	}

	/**
	 *
	 */
	virtual ~Cached()
	{
		
	}

	/**
	 * Remove whatever *this has cached and re-look up the newest value.
	 */
	virtual void Flush()
	{
		this->m_t = (*m_LookupFunction)(m_lookup);
	}

	/**
	 * For full transparency, this method must be added to all children. <br />
	 * (We don't have Class<> logic to save us from this nightmare at the common/ level). <br />
	 * @param out
	 * @param t
	 * @return out
	 */
	friend ::std::ostream& operator <<(std::ostream& out, const Cached& t)
	{
		out << t.m_t;
		return out;
	}

protected:
	LOOKUP_TYPE m_lookup;
	LookupFunction m_LookupFunction;
};

} //bio namespace
