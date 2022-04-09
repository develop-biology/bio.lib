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

#include "bio/physical/arrangement/Arrangement.h"
#include "bio/physical/arrangement/Iterator.h"
#include <limits>
#include <algorithm>

namespace bio {
namespace physical {

Arrangement::Arrangement(
	const Index expectedSize,
	std::size_t stepSize
)
	:
	m_firstFree(1),
	m_size(expectedSize + 1),
	m_tempItt(NULL)
{
	m_store = (unsigned char*)std::malloc(m_size * stepSize);
	BIO_ASSERT(m_store)
}

Arrangement::Arrangement(const Arrangement& other)
	:
	m_firstFree(other.m_firstFree),
	m_size(other.m_size),
	m_tempItt(NULL)
{
	m_store = (unsigned char*)std::malloc(m_size * other.GetStepSize());
	BIO_ASSERT(m_store)
	Import(&other); // <- NOT VIRTUAL (in ctor).
}

Arrangement::~Arrangement()
{
	Clear(); // <- NOT VIRTUAL (in dtor).
	if (m_tempItt)
	{
		delete m_tempItt;
		m_tempItt = NULL;
	}
	std::free(m_store);
}

Index Arrangement::GetBeginIndex() const
{
	return 1;
}

Index Arrangement::GetEndIndex() const
{
	return GetAllocatedSize();
}

Index Arrangement::GetCapacity() const
{
	return m_size;
}

Index Arrangement::GetAllocatedSize() const
{
	return m_firstFree;
}

Index Arrangement::GetNumberOfElements() const
{
	return GetAllocatedSize() - m_deallocated.size();
}

bool Arrangement::IsInRange(const Index index) const
{
	return index && index < m_size;
}

bool Arrangement::IsFree(Index index) const
{
	if (index >= m_firstFree)
	{
		return true;
	}
	return std::find(
		m_deallocated.begin(),
		m_deallocated.end(),
		index
	) != m_deallocated.end();
}

bool Arrangement::IsAllocated(const Index index) const
{
	return IsInRange(index) && !IsFree(index);
}

void Arrangement::Expand()
{
	BIO_SANITIZE(m_size < std::numeric_limits< Index >::max(), ,
		return)
	Index targetSize = m_size * m_size; //squared.
	if (targetSize < m_size)
	{
		targetSize = std::numeric_limits< Index >::max();
	}
	m_store = (unsigned char*)std::realloc(
		m_store,
		targetSize * GetStepSize());
	BIO_SANITIZE(m_store, ,
		return)
	m_size = targetSize;
}

Index Arrangement::Add(const ByteStream content)
{
	Index ret = GetNextAvailableIndex();
	BIO_SANITIZE(ret, ,
		return ret)
	std::memcpy(
		&m_store[ret * sizeof(ByteStream)],
		content,
		sizeof(ByteStream));
	return ret;
}

Index Arrangement::Insert(
	const ByteStream content,
	const Index index
)
{
	BIO_SANITIZE(index,,return InvalidIndex())

	if (index == m_firstFree)
	{
		//no adjustment necessary.
		Add(content);
	}

	if (GetAllocatedSize() == GetCapacity())
	{
		Expand();
	}

	//move all memory down 1.
	std::memcpy(
		&m_store[index * GetStepSize()],
		&m_store[(index + 1) * GetStepSize()],
		(m_firstFree - index) * GetStepSize());

	//adjust all deallocated positions.
	std::deque<Index> adjustedDeallocations;
	for (std::deque<Index>::iterator dlc = m_deallocated.begin(); dlc != m_deallocated.end(); ++dlc)
	{
		adjustedDeallocations.push_front(*dlc + 1);
	}
	m_deallocated = adjustedDeallocations;

	//make sure we add to the desired index.
	m_deallocated.push_front(index);

	//add the content.
	return Add(content);
}

ByteStream Arrangement::Access(const Index index)
{
	BIO_SANITIZE(IsAllocated(index), ,
		return NULL)
	ByteStream* ret;
	std::memcpy(
		ret,
		&m_store[index * sizeof(ByteStream)],
		sizeof(ByteStream));
	return *ret;
}

const ByteStream Arrangement::Access(const Index index) const
{
	BIO_SANITIZE(IsAllocated(index), ,
		return NULL)
	ByteStream* ret;
	std::memcpy(
		ret,
		&m_store[index * sizeof(ByteStream)],
		sizeof(ByteStream));
	return *ret;
}

Index Arrangement::SeekTo(const ByteStream content) const
{
	if (!m_tempItt)
	{
		m_tempItt = ConstructClassIterator();
	}
	m_tempItt->MoveTo(GetEndIndex());
	for (
		; !m_tempItt->IsAtBeginning();
		--m_tempItt
		)
	{
		if (AreEqual(
			m_tempItt->GetIndex(),
			content
		))
		{
			return m_tempItt->GetIndex();
		}
	}
	return InvalidIndex();
}

bool Arrangement::Has(const ByteStream content) const
{
	return SeekTo(content); //implicit cast to bool should work.
}

bool Arrangement::Erase(const Index index)
{
	BIO_SANITIZE(IsAllocated(index), ,
		return false)
	ByteStream* toDelete;
	std::memcpy(
		toDelete,
		&m_store[index * sizeof(ByteStream)],
		sizeof(ByteStream));
	delete toDelete;
	m_deallocated.push_back(index);
	return true;
}

void Arrangement::Import(const Arrangement* other)
{
	BIO_SANITIZE(other, ,
		return)
	Iterator* otr = other->End();
	for (
		; !otr->IsAtBeginning();
		--otr
		)
	{
		Add(**otr);
	}
}

void Arrangement::Clear()
{
	//Call destructors before clearing the array.
	if (!m_tempItt)
	{
		m_tempItt = ConstructClassIterator();
	}
	m_tempItt->MoveTo(GetEndIndex());
	for (
		; !m_tempItt->IsAtBeginning();
		--m_tempItt
		)
	{
		Erase(m_tempItt->GetIndex());
	}

	m_firstFree = 1;
	m_deallocated.clear();
}

Iterator* Arrangement::ConstructClassIterator(const Index index) const
{
	BIO_SANITIZE(IsAllocated(index), ,
		return NULL)
	return new Iterator(
		this,
		index
	);
}

SmartIterator Arrangement::Begin() const
{
	return SmartIterator(
		this,
		GetBeginIndex());
}

SmartIterator Arrangement::End() const
{
	return SmartIterator(
		this,
		GetEndIndex());
}

ByteStream Arrangement::operator[](const Index index)
{
	return Access(index);
}

const ByteStream Arrangement::operator[](const Index index) const
{
	return Access(index);
}

ByteStream Arrangement::operator[](const SmartIterator itt)
{
	return Access(itt);
}

const ByteStream Arrangement::operator[](const SmartIterator itt) const
{
	return Access(itt);
}

Index Arrangement::GetNextAvailableIndex()
{
	Index ret = InvalidIndex();
	if (!m_deallocated.empty())
	{
		ret = m_deallocated.front();
		m_deallocated.pop_front();
	}
	else if (m_firstFree == m_size)
	{
		Expand();
		ret = m_firstFree++;
	}
	return ret;
}

bool Arrangement::AreEqual(
	Index internal,
	const ByteStream external
) const
{
	return Access(internal) == external;
}

const std::size_t Arrangement::GetStepSize() const
{
	return sizeof(ByteStream);
}

} //physical namespace
} //bio namespace
