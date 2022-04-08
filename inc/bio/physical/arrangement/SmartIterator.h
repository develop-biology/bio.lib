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

class Iterator;

/**
 * SmartIterators wrap our iterator implementation to provide a consistent means of access.
 * Everything is const so that we don't need to worry about const_iterator vs iterator nonsense.
 */
class SmartIterator
{
public:

	/**
	 * @param arrangement
	 * @param index
	 */
	SmartIterator(
		const AbstractArrangement* arrangement,
		Index index = InvalidIndex());

	/**
	 * Not virtual
	 */
	~SmartIterator();

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
