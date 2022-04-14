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
#include "bio/physical/macros/Macros.h"

namespace bio {

class Container;

/**
 * An Iterator is the preferred means of accessing the elements stored in an Container.
 * Please use increment and decrement operators to move through these elements.
 * While untested, it is likely that starting at the end and decrementing will be faster than starting at the beginning and incrementing, due to removing the overhead of having to keep track of where the end is.
 *
 * NOTE: There are no checks to guard against being given a bad Container*. These have been neglected to increase performance.
 */
class Iterator
{
public:

	/**
	 * Constructor is only built with a const Container*.
	 * We cast away the cv and keep only the mutable pointer.
	 * This is done in order to avoid having a separate class for const access (as is in the standard library).
	 * @param container
	 * @param index
	 */
	Iterator(
		const Container* container,
		const Index index = InvalidIndex());

	/**
	 *
	 */
	virtual ~Iterator();

	/**
	 * @return the index *this is currently at.
	 */
	Index GetIndex() const;

	/**
	 * Make *this point somewhere else;
	 * @param index
	 * @return whether or not *this was moved.
	 */
	bool MoveTo(const Index index);

	/**
	 * @return whether or not *this has reached the beginning of its Arrangement.
	 */
	virtual bool IsAtBeginning() const;

	/**
	 * @return whether or not *this has reached the end of its Arrangement.
	 */
	virtual bool IsAtEnd() const;

	/**
	 * Move *this up a Index
	 * @return *this after incrementing.
	 */
	virtual Iterator* Increment();

	/**
	 * Move *this down a Index.
	 * @return *this following decrementing.
	 */
	virtual Iterator* Decrement();

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

protected:
	mutable Container* m_container;
	Index m_index;
}; //Iterator class.

} //bio namespace
