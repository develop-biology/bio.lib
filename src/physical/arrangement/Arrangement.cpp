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

namespace bio {
namespace physical {

static const Index Arrangement::InvalidIndex()
{
	return 0;
}

Arrangement::Arrangement(const Index expectedSize, std::size_t stepSize) :
	m_firstFree(1),
	m_size(expectedSize+1),
	m_tempItt(NULL)
{
	m_store = std::malloc(m_store, m_size * stepSize);
	BIO_ASSERT(m_store)
}

Arrangement::~Arrangement()
{
	Clear();
	std::free(m_store);
	if (m_tempItt)
	{
		delete m_tempItt;
		m_tempItt = NULL;
	}
}

Arrangement::Postion Arrangement::GetBeginIndex() const
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
	return std::find(m_deallocated.begin(), m_deallocated.end(), index) != m_deallocated.end();
}

inline bool Arrangement::IsAllocated(const Index index) const
{
	return IsInRange(index) && !IsFree(index);
}

void Arrangement::Expand(std::size_t stepSize)
{
	BIO_SANITIZE(m_size < std::numeric_limits< Index >::max(),,return)
	Index targetSize = m_size * m_size; //squared.
	if (targetSize < m_size)
	{
		targetSize = std::numeric_limits< Index >::max();
	}
	m_store = std::realloc(m_store, targetSize * stepSize);
	BIO_SANITIZE(m_store,,return)
	m_size = targetSize;
}

Index Arrangement::Add(const ByteStream content)
{
	Index ret = GetNextAvailableIndex(sizeof(ByteStream));
	BIO_SANITIZE(ret,,return ret)
	ByteStream toStore;
	toStore.Set(content);
	m_store[ret * sizeof(ByteStream)] = toStore;
	return ret;
}

ByteStream Arrangement::Access(const Index index)
{
	BIO_SANITIZE(IsAllocated(index),,return NULL)
	return Cast< ByteStream >(m_store[index * sizeof(ByteStream)]);
}

const ByteStream Arrangement::Access(const Index index) const
{
	BIO_SANITIZE(IsAllocated(index),,return NULL)
	return Cast< const ByteStream >(m_store[index * sizeof(ByteStream)]);
}

Index Arrangement::SeekTo(const ByteStream content) const
{
	Index ret = InvalidIndex();
	if (!m_tempItt)
	{
		m_tempItt = ConstructClassIterator();
	}
	m_tempItt->MoveTo(GetEndIndex());
	for (; !itt->IsAtBeginning(); --itt)
	{
		if (AreEqual(itt->GetIndex(), content))
		{
			return itt->GetIndex();
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
	BIO_SANITIZE(IsAllocated(index),,return false)
	ByteStream toDelete = Cast< ByteStream >(m_store[index * sizeof(ByteStream)]);
	~toDelete;
	m_deallocated.push(index);
	return true;
}

void Arrangement::Import(const Arrangement* other)
{
	BIO_SANITIZE(other,,return)
	Iterator* otr = other->End();
	for(;!otr->IsAtBeginning(); --otr)
	{
		Add(**otr);
	}
}

void Arrangement::Clear()
{
	//Call destructors before clearing the array.
	Iterator* otr = other->End();
	for(;!otr->IsAtBeginning(); --otr)
	{
		Erase((*otr)->GetIndex());
	}

	m_firstFree = 1;
	m_deallocated.clear();
}

Iterator* Arrangement::ConstructClassIterator(const Index index = InvalidIndex())
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
	return SmartIterator(this, GetBeginIndex());
}

SmartIterator Arrangement::End() const
{
	return SmartIterator(this, GetEndIndex());
}

ByteStream Arrangement::operator[](const Index index)
{
	return Access(index);
}

const ByteStream Arrangement::operator[](const Index index) const
{
	return Access(index);
}

Index Arrangement::GetNextAvailableIndex(std::size_t stepSize)
{
	Index ret = InvalidIndex();
	if (!m_deallocated.empty())
	{
		ret = m_deallocated.front();
		m_deallocated.pop();
	}
	else if (m_firstFree == m_size)
	{
		Expand(stepSize);
		ret = m_firstFree++;
	}
	return ret;
}

bool Arrangement::AreEqual(
	Index internal,
	ByteStream external
)
{
	return Access(internal) == external;
}
} //physical namespace
} //bio namespace
