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

#include "Arrangement.h"
#include "bio/physical/macros/Macros.h"
#include "bio/common/Cast.h"
#include <deque>
#include <algorithm>

namespace bio {
namespace physical {

/**
 * TypeOptimizedArrangements provide a more memory-efficient implementation of the Arrangement interface for a single type.
 * @tparam TYPE
 */
template < typename TYPE >
class TypeOptimizedArrangement : public Arrangement
{
public:

	/**
	 * @param expectedSize
	 */
	TypeOptimizedArrangement(const Index expectedSize=2) :
		Arrangement(expectedSize, sizeof(TYPE))
	{
		
	}

	/**
	 *
	 */
	virtual ~TypeOptimizedArrangement()
	{

	}

	Index Add(const ByteStream content)
	{
		BIO_SANITIZE(content.Is<TYPE>(),,return InvalidIndex())
		Index ret = GetNextAvailableIndex(sizeof(TYPE));
		BIO_SANITIZE(ret,,return InvalidIndex())
		TYPE toAdd = content;
		std::memcpy(&m_store[ret * sizeof(TYPE)], &toAdd, sizeof(TYPE));
		return ret;
	}

	ByteStream Access(const Index index)
	{
		BIO_SANITIZE(IsInRange(index),,return NULL)
		TYPE* ret;
		std::memcpy(ret, &m_store[index * sizeof(TYPE)], sizeof(TYPE));
		return *ret;
	}

	const ByteStream Access(const Index index) const
	{
		BIO_SANITIZE(IsInRange(index),,return NULL)
		TYPE* ret;
		std::memcpy(ret, &m_store[index * sizeof(TYPE)], sizeof(TYPE));
		return *ret;
	}

	bool Erase(const Index index)
	{
		BIO_SANITIZE(IsAllocated(index),,return false)
		TYPE* toDelete;
		std::memcpy(toDelete, &m_store[index * sizeof(TYPE)], sizeof(TYPE));
		delete toDelete;
		m_deallocated.push_back(index);
		return true;
	}

	virtual bool AreEqual(Index internal, const ByteStream external) const
	{
		BIO_SANITIZE(external.Is< TYPE >(),, return false)
		return Access(internal).template As< TYPE >() == external.template As< TYPE >();
	}
};

} //physical namespace
} //bio namespace
