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
#include "bio/common/Cast.h"
#include "SmartIterator.h"
#include <deque>

namespace bio {
namespace physical {

class Iterator;

/**
 * Arrangement exists as a base class to templated Arrangements.
 * Essentially we need a generic, non-template std::vector<> base class, so we'll make our own.
 *
 * You can think of Arrangements as our own internal RAM.
 * We allocated a block of bytes and manipulate them as necessary.
 * By default, we use ByteStreams to store arbitrary data in our allocated memory.
 * ByteStreams can be wasteful though (e.g. a whole extra std::string for every Index); so, overrides of *this can use the same internal memory block but store other data structures for more efficient memory usage.
 * We maintain ByteStreams as our data conversion type, as they are flexible but safer than a void*.
 *
 * When using Arrangements, we make no guarantees regarding the type of data stored.
 * All we provide is a consistent means of accessing those data. To this end, we ensure that a Index's validity follows the lifecycle of the datum at that Index. This is identical to pointers: a Index represents the memory address of what is stored in *this. This means that as data are erased from *this, the memory is not moved, consolidated, or manipulated in any way that destroys the old references.
 *
 * When using an Iterator, you will be given a SmartIterator which dynamically determines its implementation. Thus, we allow for full inheritance of *this base class.
 *
 * There is another tradeoff here that we are leaning into: our implementation does not make for easy use of std:: containers under-the-hood. By enforcing consistency on access, we've made the system less flexible. This may be changed in a major release down the road but its what we're sticking with for now.
 */
class Arrangement
{
public:

	/**
	 *
	 */
	Arrangement(const Index expectedSize=2, std::size_t stepSize = sizeof(ByteStream));

	/**
	 *
	 */
	virtual ~Arrangement();

	/**
	 * @return where to start.
	 */
	virtual Index GetBeginIndex() const;

	/**
	 * @return where to end.
	 */
	virtual Index GetEndIndex() const;

	/**
	 * @return the number of allocatable Indexs in *this.
	 */
	virtual Index GetCapacity() const;

	/**
	 * GetCapacity - the number of free Indexs at the end (ignores any deallocated Indexs in the middle).
	 * @return the number of Indexs that have been allocated in *this.
	 */
	virtual Index GetAllocatedSize() const;

	/**
	 * GetAllocatedSize - the number of deallocated Indexs.
	 * @return the number of elements in *this.
	 */
	virtual Index GetNumberOfElements() const;

	/**
	 * Checks if the given Index is available to be allocated, i.e. the Index should not be used.
	 * NOTE: Just because a Index is not free does not necessarily mean the Index has been allocated.
	 * @param Index
	 * @return whether or not the given Index is free to use.
	 */
	virtual bool IsFree(const Index Index) const;

	/**
	 * NOTE: Just because a Index IsInRange does not mean it is free or allocated.
	 * @param Index
	 * @return whether or not the Index is expected to yield a valid result when used with *this.
	 */
	virtual bool IsInRange(const Index Index) const;

	/**
	 * @param Index
	 * @return IsInRange && !IsFree.
	 */
	virtual bool IsAllocated(const Index Index) const;

	/**
	 * Grow store to accommodate dynamic allocation.
	 * @param datumSize the memory size a Index should jump over.
	 */
	virtual void Expand(std::size_t stepSize = sizeof(ByteStream));

	/**
	 * Adds content to *this.
	 * @param content
	 * @return the Index of the added content.
	 */
	virtual Index Add(const ByteStream content);

	/**
	 * Get access to an element.
	 * NOTE: THIS DOES NOT CHECK IF THE ELEMENT IsFree!!!
	 * Free checks can be done independently. This is done for speed.
	 * @param Index
	 * @return a pointer to the value stored in *this.
	 */
	virtual ByteStream Access(const Index Index);

	/**
	 * Get access to an element.
	 * NOTE: THIS DOES NOT CHECK IF THE ELEMENT IsFree!!!
	 * Free checks can be done independently. This is done for speed.
	 * @param Index
	 * @return a pointer to the value stored in *this.
	 */
	virtual const ByteStream Access(const Index Index) const;

	/**
	 * Find the Index of content within *this.
	 * @param content
	 * @return the Index of content within *this or InvalidIndex.
	 */
	Index SeekTo(const ByteStream content) const;

	/**
	 * @param content
	 * @return whether or not *this contains the given content.
	 */
	bool Has(const ByteStream content) const;

	/**
	 * Removes content from *this.
	 * @param Index
	 * @return whether or not the erasure was successful.
	 */
	virtual bool Erase(Index Index);

	/**
	 * Remove all elements from *this.
	 */
	virtual void Clear();

	/**
	 * Copy the contents of other into *this.
	 * @param other
	 */
	virtual void Import(const Arrangement* other);

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param Index
	 * @return Access(Index).
	 */
	virtual ByteStream operator[](const Index Index);

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param Index
	 * @return Access(Index).
	 */
	virtual const ByteStream operator[](const Index Index) const;

	/**
	 * Override this to construct Iterators for your Arrangements.
	 * @param Index
	 * @return a new Iterator pointing to the given Index in *this or NULL.
	 */
	virtual Iterator* ConstructClassIterator(const Index Index = InvalidIndex()) const;

	/**
	 * NOTE: This does not need to be overridden if you've already defined ConstructClassIterator().
	 * @return A new Iterator pointing to the beginning of *this.
	 */
	virtual SmartIterator Begin() const;

	/**
	 * NOTE: This does not need to be overridden if you've already defined ConstructClassIterator().
	 * @return An Iterator pointing to the end of *this.
	 */
	virtual SmartIterator End() const;

protected:

	/**
	 * For ease of use when Add()ing.
	 * NOTE: This will mark the returned Index as filled, so please make sure it actually receives content.
	 * @return a Index to fill with new content.
	 */
	virtual Index GetNextAvailableIndex(std::size_t stepSize);

	/**
	 * To make comparisons easier and reduce the work needed to optimize *this, children can define a comparison method which will be used for all searches.
	 * @param internal
	 * @param external
	 * @return whether or not the contents of *this at the given Index match the given datum.
	 */
	virtual bool AreEqual(Index internal, const ByteStream external) const;

	/**
	 * cannot be void* as we need an object type for pointer arithmetic.
	 */
	unsigned char* m_store;

	Index m_size;
	Index m_firstFree;
	std::deque<Index> m_deallocated;

	/**
	 * An iterator for use in loops.
	 * Matches lifecycle of object for better performance.
	 */
	mutable Iterator* m_tempItt;
};

} //physical namespace
} //bio namespace
