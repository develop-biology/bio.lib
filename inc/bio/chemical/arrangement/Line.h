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

#include "Linear.h"
#include "Arrangement.h"

namespace bio {
namespace chemical {

/**
 * Lines are Linear Arrangements.
 *
 * NOTE: We reserve Position 0 as invalid.
 *
 * @tparam STORE
 */
class Line : public Line< Linear >
{
public:

	/**
	 * @param expectedSize
	 */
	Line(Position expectedSize = 1);

	/**
	 *
	 */
	virtual ~Line();

	/**
	 * Adds content to *this.
	 * @param content
	 * @return the Position of the added content.
	 */
	virtual Position Add(physical::Identifiable< StandardDimension >* content);

	/**
	 * Copy the contents of other into *this.
	 * @param other
	 */
	virtual void Import(const Line* other);

	/**
	 * Get access to an element.
	 * NOTE: THIS DOES NOT CHECK IF THE ELEMENT IsFree!!!
	 * Free checks can be done independently. This is done for speed.
	 * @param position
	 * @return a pointer to the value stored in *this.
	 */
	virtual physical::Identifiable< StandardDimension >* Access(Position position);

	/**
	 * Get access to an element.
	 * NOTE: THIS DOES NOT CHECK IF THE ELEMENT IsFree!!!
	 * Free checks can be done independently. This is done for speed.
	 * @param position
	 * @return a pointer to the value stored in *this.
	 */
	virtual const physical::Identifiable< StandardDimension >* Access(Position position) const;

	/**
	 * Find the Position of content within *this.
	 * @param content
	 * @return the Position of content within *this or InvalidPosition.
	 */
	Position SeekTo(const physical::Identifiable< StandardDimension >* content) const;

	/**
	 * @param content
	 * @return whether or not *this contains the given content.
	 */
	bool Has(const physical::Identifiable< StandardDimension >* content) const;

	/**
	 * SeekTo and Access some content.
	 * @param content
	 * @return a pointer to a matching value in *this.
	 */
	const physical::Identifiable< StandardDimension >* GetInternalPointer(const physical::Identifiable< StandardDimension >* content) const;

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param position
	 * @return Access(position).
	 */
	virtual physical::Identifiable< StandardDimension >* operator[](Position position);

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param position
	 * @return Access(position).
	 */
	virtual const physical::Identifiable< StandardDimension >* operator[](Position position) const;

	/**
	 * Specialized Iterator, for properly dereferencing elements of Line<>.
	 */
	class Iterator : public Arrangement< Linear >::Iterator
	{
	public:
		/**
		 * @param Line
		 * @param position
		 */
		Iterator(const Line* line, Position position = InvalidPosition());

		/**
		 *
		 */
		virtual ~Iterator();

		/**
		 * @return whatever is stored in the Line *this is iterating over at the Position *this is currently at.
		 */
		virtual physical::Identifiable< StandardDimension >* operator*();

		/**
		 * @return whatever is stored in the Line *this is iterating over at the Position *this is currently at.
		 */
		virtual const physical::Identifiable< StandardDimension >* operator*() const;

		/**
		 * @return whatever is stored in the Line *this is iterating over at the Position *this is currently at.
		 */
		virtual physical::Identifiable< StandardDimension >* operator->();

		/**
		 * @return whatever is stored in the Line *this is iterating over at the Position *this is currently at.
		 */
		virtual const physical::Identifiable< StandardDimension >* operator->() const;
	};

	/**
	 * Override this to construct Iterators for your Lines.
	 * NOTE: This code is the same as AbstractLine but the "namespace" has changed. This returns an Line<>::Iterator, not an AbstractLine::Iterator.
	 * @param position
	 * @return a new Iterator pointing to the given position in *this or NULL.
	 */
	virtual AbstractArrangement::Iterator* ConstructClassIterator(Position position=InvalidPosition());
};

} //chemical namespace
} //bio namespace
