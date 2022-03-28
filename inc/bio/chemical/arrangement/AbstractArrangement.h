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
	virtual Position GetTotalSize() const
	{
		return InvalidPosition();
	}

	/**
	 * GetTotalSize - the number of free Positions at the end (ignores any deallocated Positions in the middle).
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
	virtual Position GetUsedSize() const
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
		virtual bool IsAtBeginning()
		{
			return !m_position;
		}

		/**
		 * @return whether or not *this has reached the end of its Arrangement.
		 */
		virtual bool IsAtEnd()
		{
			return m_position == m_arrangement->GetAllocatedSize();
		}

		//PLEASE DEFINE THE FOLLOWING FUNCTIONS IN YOUR CHILD ITERATOR
		//replace "void" with the appropriate type.

		#if 0
		/**
		 * @return whatever is stored in the Arrangement *this is iterating over at the Position *this is currently at.
		 */
		virtual void& operator*()
		{
			return;
		}

		/**
		 * @return whatever is stored in the Arrangement *this is iterating over at the Position *this is currently at.
		 */
		virtual const void& operator*() const
		{
			return;
		}

		/**
		 * @return whatever is stored in the Arrangement *this is iterating over at the Position *this is currently at.
		 */
		virtual void* operator->()
		{
			return NULL;
		}

		/**
		 * @return whatever is stored in the Arrangement *this is iterating over at the Position *this is currently at.
		 */
		virtual const void* operator->() const
		{
			return NULL;
		}
		#endif

		/**
		 * @return *this after incrementing.
		 */
		virtual Iterator& operator++()
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
			return *this;
		}

		/**
		 * @return a copy of *this before incrementing.
		 */
		virtual Iterator operator++(int)
		{
			Iterator ret = *this;
			++*this;
			return ret;
		}

		/**
		 * @return *this following decrementing.
		 */
		virtual Iterator& operator--()
		{
			if (!m_position)
			{
				return *this;
			}
			while (m_arrangement->IsFree(--m_position) && m_position > 0)
			{
				continue; //avoid re-referencing m_position; see condition.
			}
			return *this;
		}

		/**
		 * @return a copy of *this before decrementing.
		 */
		virtual Iterator operator--(int)
		{
			Iterator ret = *this;
			--*this;
			return ret;
		}

	protected:
		AbstractArrangement* m_arrangement;
		Position m_position;
	}; //Iterator class.

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
	virtual Iterator* Begin() const
	{
		return ConstructClassIterator(GetBeginPosition());
	}

	/**
	 * NOTE: This does not need to be overridden if you've already defined ConstructClassIterator().
	 * @tparam T the kind of Iterator you expect.
	 * @return A new Iterator pointing to the beginning of *this.
	 */
	template < typename T >
	virtual Iterator* Begin() const
	{
		return Cast< T >(ConstructClassIterator(GetBeginPosition()));
	}

	/**
	 * NOTE: This does not need to be overridden if you've already defined ConstructClassIterator().
	 * @return An Iterator pointing to the end of *this.
	 */
	virtual Iterator* End() const
	{
		return ConstructClassIterator(GetEndPosition());
	}

	/**
	 * NOTE: This does not need to be overridden if you've already defined ConstructClassIterator().
	 * @tparam T the kind of iterator you expect.
	 * @return An Iterator pointing to the end of *this.
	 */
	template < typename T >
	virtual Iterator* End() const
	{
		return Cast< T >(ConstructClassIterator(GetEndPosition()));
	}
};

} //chemical namespace
} //bio namespace
