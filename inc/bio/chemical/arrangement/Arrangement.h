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

#include "AbstractArrangement.h"
#include "bio/chemical/macros/Macros.h"
#include "bio/chemical/common/Cast.h"
#include <deque>
#include <algorithm>

namespace bio {
namespace chemical {

/**
 * Arrangements are the Biology equivalent to std::vector.
 * We roll our own implementation for inheritance reasons.
 * Hopefully, we can optimize this a bit more too but that is secondary to achieving basic functionality.
 *
 * NOTE: We reserve Position 0 as invalid.
 *
 * @tparam STORE
 */
template < typename STORE >
class Arrangement : public AbstractArrangement
{
public:

	/**
	 * @param expectedSize
	 */
	Arrangement(Position expectedSize = 1) :
		m_firstFree(1),
		m_size(expectedSize+1),
		m_tempItt(this->ConstructClassIterator())
	{
		this->m_store = new STORE[m_size];
	}

	/**
	 *
	 */
	virtual ~Arrangement()
	{
		delete[] this->m_store;
		delete m_tempItt;
	}

	/**
	 * @return the number of allocatable Positions in *this.
	 */
	virtual Position GetCapacity() const
	{
		return m_size;
	}

	/**
	 * GetCapacity - the number of free Positions at the end (ignores any allocated Positions in the middle).
	 * @return the number of Positions that have been allocated in *this.
	 */
	virtual Position GetAllocatedSize() const
	{
		return m_firstFree;
	}

	/**
	 * GetAllocatedSize - the number of deallocated Positions.
	 * @return the number of elements in *this.
	 */
	virtual Position GetNumberOfElements() const
	{
		return this->GetAllocatedSize() - this->m_deallocated.size();
	}

	/**
	 * @param position
	 * @return whether or not the position is applicable to *this.
	 */
	virtual inline bool IsValid(const Position position) const
	{
		return position && position < this->m_size;
	}

	/**
	 * Checks if the given position is available to be allocated, i.e. the position should not be used.
	 * NOTE: Just because a position is free does not mean it is valid (i.e. has been allocated)!
	 * @param position
	 * @return whether or not the given position can be safely written to.
	 */
	virtual bool IsFree(Position position) const
	{
		if (position >= m_firstFree)
		{
			return true;
		}
		return std::find(this->m_deallocated.begin(), this->m_deallocated.end(), position) != this->m_deallocated.end();
	}


	/**
	 * Grow storage to accommodate dynamic allocation.
	 */
	virtual void Expand()
	{
		Position targetSize = this->m_size * this->m_size;
		STORE* expanded = new STORE[targetSize];
		memcpy(expanded, this->m_store, this->m_size * sizeof(STORE));
		free(this->m_store);
		this->m_store = expanded;
		this->m_size = targetSize;
	}

	/**
	 * Adds content to *this.
	 * @param content
	 * @return the Position of the added content.
	 */
	virtual Position Add(STORE content)
	{
		Position ret;
		if (!this->m_deallocated.empty())
		{
			ret = this->m_deallocated.front();
			this->m_deallocated.pop();
			this->m_store[ret] = content;
			return ret;
		}
		if (this->m_firstFree == this->m_size)
		{
			this->Expand();
		}
		ret = this->m_firstFree++;
		this->m_store[ret] = content;
		return ret;
	}

	/**
	 * Copy the contents of other into *this.
	 * @param other
	 */
	virtual void Import(const Arrangement< STORE >* other)
	{
		BIO_SANITIZE(other,,return)

		Iterator* otr = other->End();
		for(;!otr->IsAtBeginning(); --otr)
		{
			this->Add(**otr);
		}
	}

	/**
	 * Removes content from *this.
	 * @param position
	 * @return whether or not the erasure was successful.
	 */
	virtual bool Erase(Position position)
	{
		BIO_SANITIZE(!this->IsValid(position),,return false)
		BIO_SANITIZE(!this->IsFree(position),,return false)

		~m_store[position];
		this->m_deallocated.push(position);

		return true;
	}

	/**
	 * Remove all elements from *this.
	 */
	virtual void Clear()
	{
		//Call destructors before clearing the array.
		Iterator* otr = other->End();
		for(;!otr->IsAtBeginning(); --otr)
		{
			this->~m_store[(*otr)->GetPosition())];
		}

		this->m_firstFree = 1;
		this->m_deallocated.clear();
	}

	/**
	 * Get access to an element.
	 * NOTE: THIS DOES NOT CHECK IF THE ELEMENT IsFree!!!
	 * Free checks can be done independently. This is done for speed.
	 * @param position
	 * @return a pointer to the value stored in *this.
	 */
	virtual STORE* Access(Position position)
	{
		BIO_SANITIZE(this->IsValid(position),,return NULL)
		return this->m_store[position];
	}

	/**
	 * Get access to an element.
	 * NOTE: THIS DOES NOT CHECK IF THE ELEMENT IsFree!!!
	 * Free checks can be done independently. This is done for speed.
	 * @param position
	 * @return a pointer to the value stored in *this.
	 */
	virtual const STORE* Access(Position position) const
	{
		BIO_SANITIZE(this->IsValid(position),,return NULL)
		return this->m_store[position];
	}

	/**
	 * Find the Position of content within *this.
	 * @param content
	 * @return the Position of content within *this or InvalidPosition.
	 */
	Position SeekTo(const STORE content) const
	{
		Position ret = InvalidPosition();
		m_tempItt->MoveTo(this->GetEndPosition());
		for (; !itt->IsAtBeginning(); --itt)
		{
			if (**itt == content)
			{
				return itt->GetPosition();
			}
		}
		return InvalidPosition();
	}

	/**
	 * @param content
	 * @return whether or not *this contains the given content.
	 */
	bool Has(const STORE content) const
	{
		return this->SeekTo(content); //implicit cast to bool should work.
	}

	/**
	 * SeekTo and Access some content.
	 * @param content
	 * @return a pointer to a matching value in *this.
	 */
	STORE* GetInternalPointer(const STORE content) const
	{
		return this->Access(this->SeekTo(content));
	}

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param position
	 * @return Access(position).
	 */
	virtual STORE* operator[](Position position)
	{
		return this->Access(position);
	}

	/**
	 * Ease of use wrapper around Access.
	 * See Access for details.
	 * @param position
	 * @return Access(position).
	 */
	virtual const STORE* operator[](Position position) const
	{
		return this->Access(position);
	}

	/**
	 * Specialized Iterator, for properly dereferencing elements of Arrangement<>.
	 */
	class Iterator : public AbstractArrangement::Iterator
	{
	public:
		/**
		 * @param arrangement
		 * @param position
		 */
		Iterator(const Arrangement<STORE>* arrangement, Position position = InvalidPosition()) :
			AbstractArrangement::Iterator(arrangement, position)
		{

		}

		/**
		 *
		 */
		virtual ~Iterator()
		{

		}

		/**
		 * @return whatever is stored in the Arrangement *this is iterating over at the Position *this is currently at.
		 */
		virtual STORE& operator*()
		{
			return *(Cast< Arrangement<STORE>* >(m_arrangement)->Access(m_position));
		}

		/**
		 * @return whatever is stored in the Arrangement *this is iterating over at the Position *this is currently at.
		 */
		virtual const STORE& operator*() const
		{
			return *(Cast< Arrangement<STORE>* >(m_arrangement)->Access(m_position));
		}

		/**
		 * @return whatever is stored in the Arrangement *this is iterating over at the Position *this is currently at.
		 */
		virtual STORE* operator->()
		{
			return Cast< Arrangement<STORE>* >(m_arrangement)->Access(m_position);
		}

		/**
		 * @return whatever is stored in the Arrangement *this is iterating over at the Position *this is currently at.
		 */
		virtual const STORE* operator->() const
		{
			return Cast< Arrangement<STORE>* >(m_arrangement)->Access(m_position);
		}
	};

	/**
	 * Override this to construct Iterators for your Arrangements.
	 * NOTE: This code is the same as AbstractArrangement but the "namespace" has changed. This returns an Arrangement<>::Iterator, not an AbstractArrangement::Iterator.
	 * @param position
	 * @return a new Iterator pointing to the given position in *this or NULL.
	 */
	virtual AbstractArrangement::Iterator* ConstructClassIterator(Position position=InvalidPosition())
	{
		BIO_SANITIZE(IsValid(position),,return NULL)
		return new Iterator(this, position);
	}

protected:
	STORE* m_store;
	Position m_size;
	Position m_firstFree;
	std::deque<Position> m_deallocated;

	/**
	 * An iterator for use in loops.
	 * Matches lifecycle of object for better performance.
	 */
	mutable Iterator* m_tempItt;
};

} //chemical namespace
} //bio namespace
