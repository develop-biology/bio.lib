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

#include "bio/common/Types.h"
#include "bio/common/macros/Macros.h"
#include "bio/common/Cast.h"
#include "SmartIterator.h"
#include <deque>

namespace bio {

class Iterator;

/**
 * Container exists as a base class to templated Arrangements.
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
 * This rule does have some exceptions and you are allowed to break it yourself. However, we try to stick by this as much as possible (e.g. see Insert(), below).
 *
 * When using an Iterator, you will be given a SmartIterator which dynamically determines its interface. Thus, we allow for full inheritance of *this base class.
 *
 * There is another tradeoff here that we are leaning into: our interface does not make for easy use of std:: containers under-the-hood. By enforcing consistency on access, we've made the system less flexible. This may be changed in a major release down the road but its what we're sticking with for now.
 */
class Container
{
public:

	/**
	 * NOTE: We cannot use GetStepSize() here as virtual functions are not available to ctors.
	 */
	Container(const Index expectedSize=2, std::size_t stepSize = sizeof(ByteStream));

	/**
	 * Copy ctor.
	 * Imports all contents from other into *this.
	 * @param other 
	 */
	Container(const Container& other);
	
	/**
	 *
	 */
	virtual ~Container();

	/**
	 * @return where to start.
	 */
	virtual Index GetBeginIndex() const;

	/**
	 * @return where to end.
	 */
	virtual Index GetEndIndex() const;

	/**
	 * @return the number of allocatable Indices in *this.
	 */
	virtual Index GetCapacity() const;

	/**
	 * GetCapacity - the number of free Indices at the end (ignores any deallocated Indices in the middle).
	 * @return the number of Indices that have been allocated in *this.
	 */
	virtual Index GetAllocatedSize() const;

	/**
	 * GetAllocatedSize - the number of deallocated Indices.
	 * @return the number of elements in *this.
	 */
	virtual Index GetNumberOfElements() const;

	/**
	 * Checks if the given Index is available to be allocated, i.e. the Index should not be used.
	 * NOTE: Just because a Index is not free does not necessarily mean the Index has been allocated.
	 * @param Index
	 * @return whether or not the given Index is free to use.
	 */
	virtual bool IsFree(const Index index) const;

	/**
	 * NOTE: Just because a Index IsInRange does not mean it is free or allocated.
	 * @param Index
	 * @return whether or not the Index is expected to yield a valid result when used with *this.
	 */
	virtual bool IsInRange(const Index index) const;

	/**
	 * @param Index
	 * @return IsInRange && !IsFree.
	 */
	virtual bool IsAllocated(const Index index) const;

	/**
	 * Grow store to accommodate dynamic allocation.
	 * @param datumSize the memory size a Index should jump over.
	 */
	virtual void Expand();

	/**
	 * Adds content to *this.
	 * @param content
	 * @return the Index of the added content.
	 */
	virtual Index Add(const ByteStream content);

	/**
	 * Adds content to *this at the specified position.
	 * All content past the given position is shifted down.
	 * NOTE: This explicitly breaks our rule about Indices being preserved. However, this logic is necessary if the items being inserted need to be accessed in the specified order; for example: the items in *this are molecular::Proteins that have a set execution order.
	 *
	 * To make implementing your own Arrangements easier, this method simply hacks the GetNextAvailableIndex method to return the desired index and then calls Add. Thus, by implementing Add, you also implement Insert.
	 *
	 * @param content
	 * @return the Index of the added content.
	 */
	virtual Index Insert(const ByteStream content, const Index index);

	/**
	 * Get access to an element.
	 * NOTE: THIS DOES NOT CHECK IF THE ELEMENT IsFree!!!
	 * Free checks can be done independently. This is done for speed.
	 * @param index
	 * @return the value stored in *this at the given index.
	 */
	virtual ByteStream Access(const Index index);

	/**
	 * Get access to an element.
	 * NOTE: THIS DOES NOT CHECK IF THE ELEMENT IsFree!!!
	 * Free checks can be done independently. This is done for speed.
	 * @param index
	 * @return the value stored in *this at the given index.
	 */
	virtual const ByteStream Access(const Index index) const;

	/**
	 * Access wrapper for SmartIterators.
	 * @param index
	 * @return the value stored in *this at the given index.
	 */
	inline ByteStream Access(const SmartIterator itt)
	{
		return Access(itt.GetIndex());
	}

	/**
	 * Access wrapper for SmartIterators.
	 * @param index
	 * @return the value stored in *this at the given index.
	 */
	virtual const ByteStream Access(const SmartIterator itt) const
	{
		return Access(itt.GetIndex());
	}

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
	virtual bool Erase(Index index);

	/**
	 * Erase wrapper for SmartIterators.
	 * @param itt
	 * @return whether or not the erasure was successful.
	 */
	inline bool Erase(const SmartIterator itt)
	{
		return Erase(itt.GetIndex());
	}

	/**
	 * Remove all elements from *this.
	 */
	virtual void Clear();

	/**
	 * Copy the contents of other into *this.
	 * @param other
	 */
	virtual void Import(const Container* other);

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param Index
	 * @return Access(Index).
	 */
	virtual ByteStream operator[](const Index index);

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param Index
	 * @return Access(Index).
	 */
	virtual const ByteStream operator[](const Index index) const;

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param Index
	 * @return Access(itt).
	 */
	virtual ByteStream operator[](const SmartIterator itt);

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param Index
	 * @return Access(itt).
	 */
	virtual const ByteStream operator[](const SmartIterator itt) const;

	/**
	 * Override this to construct Iterators for your Arrangements.
	 * @param Index
	 * @return a new Iterator pointing to the given Index in *this or NULL.
	 */
	virtual Iterator* ConstructClassIterator(const Index index = InvalidIndex()) const;

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
	 * Please override this to return the size of the type your Container interface is working with.
	 * @return the size of the data type stored in *this.
	 */
	virtual const std::size_t GetStepSize() const;

	/**
	 * For ease of use when Add()ing.
	 * NOTE: This will mark the returned Index as filled, so please make sure it actually receives content.
	 * @return a Index to fill with new content.
	 */
	virtual Index GetNextAvailableIndex();

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

} //bio namespace
