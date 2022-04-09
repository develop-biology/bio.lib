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

#include "bio/physical/common/Types.h"

namespace bio {
namespace physical {

class Arrangement;
class Iterator;

/**
 * SmartIterators wrap our iterator implementation to provide a consistent means of access.
 * Everything is const so that we don't need to worry about const_iterator vs iterator nonsense.
 */
class SmartIterator
{
public:

	/**
	 * Sets m_index to arrangement->GetEndIndex().
	 * @param arrangement
	 */
	SmartIterator(const Arrangement* arrangement);

	/**
	 * @param arrangement
	 * @param index
	 */
	SmartIterator(
		const Arrangement* arrangement,
		Index index);

	/**
	 * Not virtual
	 */
	~SmartIterator();

	/**
	 * @return the implementation used by *this.
	 */
	operator Iterator*();

	/**
	 * @return the index *this is currently at.
	 */
	Index GetIndex() const;

	/**
	 * Make *this point somewhere else;
	 * @param index
	 * @return whether or not *this was moved.
	 */
	bool MoveTo(Index index) const;

	/**
	 * @return whether or not *this has reached the beginning of its TypeOptimizedArrangement.
	 */
	bool IsAtBeginning() const;

	/**
	 * @return whether or not *this has reached the end of its TypeOptimizedArrangement.
	 */
	bool IsAtEnd() const;

	/**
	 * Dereferencing gives the datum *this is currently pointing to.
	 * @return a ByteStream containing the datum requested.
	 */
	virtual ByteStream operator*();

	/**
	 * Dereferencing gives the datum *this is currently pointing to.
	 * @return a ByteStream containing the datum requested.
	 */
	virtual const ByteStream operator*() const;

	/**
	 * Convenient casting wrapper.
	 * @tparam T
	 * @return *this casted to the given value.
	 */
	template < typename T >
	T As()
	{
		return **this.template As< T >();
	}

	/**
	 * Convenient casting wrapper.
	 * @tparam T
	 * @return *this casted to the given value.
	 */
	template < typename T >
	const T As() const
	{
		return **this.template As< T >();
	}

	/**
	 * Convenient casting wrapper.
	 * @tparam T
	 * @return *this casted to the given value.
	 */
	template < typename T >
	operator T()
	{
		return As< T >();
	}

	/**
	 * Convenient casting wrapper.
	 * @tparam T
	 * @return *this casted to the given value.
	 */
	template < typename T >
	operator const T() const
	{
		return As< T >();
	}

	/**
	 * Convenient casting wrapper.
	 * @tparam T
	 * @return *this casted to the given value.
	 */
	template < typename T >
	operator T*()
	{
		return &(**this.template As< T >());
	}

	/**
	 * Convenient casting wrapper.
	 * @tparam T
	 * @return *this casted to the given value.
	 */
	template < typename T >
	operator const T*() const
	{
		return &(**this.template As< T >());
	}

	/**
	 * @return *this after incrementing.
	 */
	SmartIterator& operator++() const;

	/**
	 * @return a copy of *this before incrementing.
	 */
	SmartIterator operator++(int) const;

	/**
	 * @return *this following decrementing.
	 */
	SmartIterator& operator--() const;

	/**
	 * @return a copy of *this before decrementing.
	 */
	SmartIterator operator--(int) const;

protected:
	/**
	 * Whatever. Make it mutable. I don't care.
	 */
	mutable Iterator* m_implementation;
};

} //physical namespace
} //bio namespace
