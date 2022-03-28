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

#include "bio/chemical/common/Types.h"
#include "bio/chemical/common/Cast.h"
#include "bio/chemical/macros/Macros.h"

namespace bio {
namespace chemical {

/**
 * AbstractArrangement exists as a base class to templated Arrangements.
 * Essentially we need a generic, non-template std::vector<> base class, so we'll make our own.
 *
 * Arrangements create Structure.
 *
 * You can think of Arrangements as our own internal RAM.
 *
 * When using Arrangements, we make no guarantees regarding the type of data stored. All we provide is a consistent means of accessing those data. To this end, we ensure that a Position's validity follows the lifecycle of the datum at that Position. This is identical to pointers: a Position represents the memory address of what is stored in *this. This means that as data are erased from *this, the memory is not moved, consolidated, or manipulated in any way that destroys the old references.
 *
 * When using an Iterator, you will be given a SmartIterator which dynamically determines its implementation. Thus, we allow for full inheritance of *this base class.
 *
 * There is another tradeoff here that we are leaning into: our implementation does not make for easy use of std:: containers under-the-hood. By enforcing consistency on access, we've made the system less flexible. This may be changed in a major release down the road but its what we're sticking with for now.
 */
class AbstractArrangement
{
public:
	typedef uint32_t Position;

	/**
	 * We sacrifice our first storage space for the ability to do error checking.
	 * @return a Position that holds no content.
	 */
	static const Position InvalidPosition()
	{
		return 0;
	}

	/**
	 *
	 */
	AbstractArrangement()
	{

	}

	/**
	 *
	 */
	virtual ~AbstractArrangement()
	{

	}

	/**
	 * @return where to start.
	 */
	virtual Postion GetBeginPosition() const
	{
		return 1;
	}

	/**
	 * @return where to end.
	 */
	virtual Position GetEndPosition() const
	{
		return GetAllocatedSize();
	}

	/**
	 * @return the number of allocatable Positions in *this.
	 */
	virtual Position GetCapacity() const
	{
		return InvalidPosition();
	}

	/**
	 * GetCapacity - the number of free Positions at the end (ignores any deallocated Positions in the middle).
	 * @return the number of Positions that have been allocated in *this.
	 */
	virtual Position GetAllocatedSize() const
	{
		return InvalidPosition();
	}

	/**
	 * GetAllocatedSize - the number of deallocated Positions.
	 * @return the number of elements in *this.
	 */
	virtual Position GetNumberOfElements() const
	{
		return InvalidPosition();
	}

	/**
	 * NOTE: Position validity has no bearing on whether or not the Position is free or allocated.
	 * @param position
	 * @return whether or not the position is expected to yield a valid result when used with *this.
	 */
	virtual inline bool IsValid(const Position position) const
	{
		return false;
	}

	/**
	 * Checks if the given position is available to be allocated, i.e. the position should not be used.
	 * NOTE: Just because a position is not free does not necessarily mean the position has been allocated.
	 * @param position
	 * @return whether or not the given position is free to use.
	 */
	virtual bool IsFree(Position position) const
	{
		return false;
	}

	/**
	 * Removes content from *this.
	 * @param position
	 * @return whether or not the erasure was successful.
	 */
	virtual bool Erase(Position position)
	{
		return false;
	}

	/**
	 * Remove all elements from *this.
	 */
	virtual void Clear()
	{
		//nop.
	}

	/**
	 * An AbstractArrangement::Iterator is the preferred means of accessing the elements stored in an AbstractArrangement.
	 * Please use increment and decrement operators to move through these elements.
	 * While untested, it is likely that starting at the end and decrementing will be faster than starting at the beginning and incrementing, due to removing the overhead of having to keep track of where the end is.
	 *
	 * NOTE: There are no checks to guard against being given a bad AbstractArrangement*. These have been neglected to increase performance.
	 */
	class Iterator
	{
	public:

		/**
		 * Constructor is only built with a const AbstractArrangement*.
		 * We cast away the cv and keep only the mutable pointer.
		 * This is done in order to avoid having a separate class for const access (as is in the standard library).
		 * @param arrangement
		 * @param position
		 */
		Iterator(
			const AbstractArrangement* arrangement,
			Position position = InvalidPosition())
			:
			m_arrangment(const_cast< AbstractArrangement* >(arrangment)),
			m_position(position)
		{

		}

		/**
		 *
		 */
		virtual ~Iterator()
		{

		}

		/**
		 * @return the position *this is currently at.
		 */
		Position GetPosition() const
		{
			return m_position;
		}

		/**
		 * Make *this point somewhere else;
		 * @param position
		 * @return whether or not *this was moved.
		 */
		bool MoveTo(Position position)
		{
			if (m_arrangement->IsValid(position) && !m_arrangement->IsFree(position))
			{
				m_position = position;
				return true;
			}
			return false;
		}

		/**
		 * @return whether or not *this has reached the beginning of its Arrangement.
		 */
		virtual bool IsAtBeginning() const
		{
			return !m_position;
		}

		/**
		 * @return whether or not *this has reached the end of its Arrangement.
		 */
		virtual bool IsAtEnd() const
		{
			return m_position == m_arrangement->GetAllocatedSize();
		}

		/**
		 * Move *this up a Position
		 * @return *this after incrementing.
		 */
		virtual Iterator* Increment()
		{
			if (m_position >= m_arrangement->GetAllocatedSize())
			{
				m_position = m_arrangement->GetAllocatedSize();
				return *this;
			}
			while (m_arrangement->IsFree(++m_position) && m_position < m_arrangement->GetAllocatedSize())
			{
				continue; //avoid re-referencing m_position; see condition.
			}
			return this;
		}

		/**
		 * Move *this down a Position.
		 * @return *this following decrementing.
		 */
		virtual Iterator* Decrement()
		{
			if (!m_position)
			{
				return *this;
			}
			while (m_arrangement->IsFree(--m_position) && m_position > 0)
			{
				continue; //avoid re-referencing m_position; see condition.
			}
			return this;
		}

		/**
		 * Using -> should give you the pointer type you expect; we just don't know what that is yet.
		 * @return Wrapper type for drill-down
		 */
		virtual TYPE_WRAPPER_OF_SOME_SORT& operator->()
		{
			return *TYPE_WRAPPER_OF_SOME_SORT;
		}

		/**
		 * Using -> should give you the pointer type you expect; we just don't know what that is yet.
		 * @return Wrapper type for drill-down
		 */
		virtual const TYPE_WRAPPER_OF_SOME_SORT& operator->() const
		{
			return *TYPE_WRAPPER_OF_SOME_SORT;
		}

	protected:
		mutable AbstractArrangement* m_arrangement;
		Position m_position;
	}; //Iterator class.

	/**
	 * SmartIterators wrap our iterator implementation to provide a consistent means of access.
	 * Everything is const so that we don't need to worry about const_iterator vs iterator nonsense.
	 */
	class SmartIterator
	{
	public:

		/**
		 * @param arrangement
		 * @param position
		 */
		SmartIterator(
			const AbstractArrangement* arrangement,
			Position position = InvalidPosition())
			:
			m_implementation(arrangment->ConstructClassIterator(position))
		{

		}

		/**
		 * Not virtual
		 */
		~SmartIterator()
		{
			delete m_implementation;
		}

		/**
		 * @return the position *this is currently at.
		 */
		Position GetPosition() const
		{
			return m_implementation->GetPosition();
		}

		/**
		 * Make *this point somewhere else;
		 * @param position
		 * @return whether or not *this was moved.
		 */
		bool MoveTo(Position position) const
		{
			return m_implementation->MoveTo(position);
		}

		/**
		 * @return whether or not *this has reached the beginning of its Arrangement.
		 */
		bool IsAtBeginning() const
		{
			return m_implementation->IsAtBeginning();
		}

		/**
		 * @return whether or not *this has reached the end of its Arrangement.
		 */
		bool IsAtEnd() const
		{
			return m_implementation->IsAtEnd();
		}

		/**
		 * Using -> should give you the pointer type you expect; we just don't know what that is yet.
		 * @return Implementation for drill-down
		 */
		Iterator& operator->()
		{
			return *m_implementation;
		}

		/**
		 * Using -> should give you the pointer type you expect; we just don't know what that is yet.
		 * @return Implementation for drill-down
		 */
		const Iterator& operator->() const
		{
			return *m_implementation;
		}

		/**
		 * @return *this after incrementing.
		 */
		SmartIterator& operator++() const
		{
			m_implementation->Increment();
			return *this;
		}

		/**
		 * @return a copy of *this before incrementing.
		 */
		SmartIterator operator++(int) const
		{
			SmartIterator ret = *this;
			m_implementation->Increment();
			return ret;
		}

		/**
		 * @return *this following decrementing.
		 */
		SmartIterator& operator--() const
		{
			m_implementation->Decrement();
			return *this;
		}

		/**
		 * @return a copy of *this before decrementing.
		 */
		SmartIterator operator--(int) const
		{
			SmartIterator ret = *this;
			m_implementation->Decrement();
			return ret;
		}

	protected:
		/**
		 * Whatever. Make it mutable. I don't care.
		 */
		mutable AbstractArrangement::Iterator* m_implementation;
	};


	/**
	 * Override this to construct Iterators for your Arrangements.
	 * @param position
	 * @return a new Iterator pointing to the given position in *this or NULL.
	 */
	virtual Iterator* ConstructClassIterator(Position position = InvalidPosition())
	{
		BIO_SANITIZE(IsValid(position), ,
			return NULL)
		return new Iterator(
			this,
			position
		);
	}

	/**
	 * NOTE: This does not need to be overridden if you've already defined ConstructClassIterator().
	 * @return A new Iterator pointing to the beginning of *this.
	 */
	virtual SmartIterator Begin() const
	{
		return SmartIterator(this, GetBeginPosition());
	}

	/**
	 * NOTE: This does not need to be overridden if you've already defined ConstructClassIterator().
	 * @return An Iterator pointing to the end of *this.
	 */
	virtual SmartIterator End() const
	{
		return SmartIterator(this, GetEndPosition());
	}
};

} //chemical namespace
} //bio namespace
