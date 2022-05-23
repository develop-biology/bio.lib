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

#include "bio/common/container/Container.h"
#include "bio/common/container/Iterator.h"
#include <limits>
#include <algorithm>

namespace bio {

Container::Container(
	const Index expectedSize,
	std::size_t stepSize
)
	:
	mFirstFree(1),
	mSize(expectedSize + 1)
{
	LockThread();
	mStore = (unsigned char*)std::malloc(mSize * stepSize);
	UnlockThread();
	BIO_ASSERT(mStore)
}

Container::Container(const Container& other)
	:
	mFirstFree(other.mFirstFree),
	mSize(other.mSize),
	mDeallocated(other.mDeallocated)
{
	LockThread();
	mStore = (unsigned char*)std::malloc(mSize * other.GetStepSize());
	BIO_ASSERT(mStore)
	std::memcpy(
		mStore,
		other.mStore,
		mFirstFree * other.GetStepSize());
	UnlockThread();
}

Container::Container(const Container* other)
	:
	mFirstFree(other->mFirstFree),
	mSize(other->mSize),
	mDeallocated(other->mDeallocated)
{
	LockThread();
	mStore = (unsigned char*)std::malloc(mSize * other->GetStepSize());
	BIO_ASSERT(mStore)
	std::memcpy(
		mStore,
		other->mStore,
		mFirstFree * other->GetStepSize());
	UnlockThread();
}

Container::~Container()
{
	//Clearing *this is currently not needed nor useful.
	//Clear(); // <- NOT VIRTUAL (in dtor).
	if (mStore)
	{
		LockThread();
		std::free(mStore);
		mStore = NULL;
		UnlockThread();
	}
}

Index Container::GetBeginIndex() const
{
	Index ret = 1;
	while (IsFree(ret))
	{
		if (!IsInRange(++ret))
		{
			return InvalidIndex();
		}
	}
	return ret;
}

Index Container::GetEndIndex() const
{
	Index ret = GetAllocatedSize();
	if (!ret)
	{
		return InvalidIndex();
	}
	while (IsFree(ret))
	{
		if (!--ret)
		{
			return InvalidIndex();
		}
	}
	return ret;
}

Index Container::GetCapacity() const
{
	return mSize - 1;
}

Index Container::GetAllocatedSize() const
{
	return mFirstFree - 1; //last not free.
}

Index Container::GetNumberOfElements() const
{
	BIO_ASSERT(GetAllocatedSize() >= mDeallocated.size())
	return GetAllocatedSize() - mDeallocated.size();
}

bool Container::IsInRange(const Index index) const
{
	return index && index <= GetCapacity();
}

bool Container::IsFree(Index index) const
{
	//InvalidIndex should always be free.
	if (!index)
	{
		return true;
	}

	if (index >= mFirstFree)
	{
		return true;
	}

	return ::std::find(
		mDeallocated.begin(),
		mDeallocated.end(),
		index
	) != mDeallocated.end();
}

bool Container::IsAllocated(const Index index) const
{
	return IsInRange(index) && !IsFree(index);
}

void Container::Expand()
{
	BIO_SANITIZE(mSize != ::std::numeric_limits< Index >::max(), ,
		return)
	Index targetSize = mSize * mSize; //squared. 
	if (targetSize < mSize)
	{
		targetSize = ::std::numeric_limits< Index >::max();
	}
	LockThread();
	mStore = (unsigned char*)std::realloc(
		mStore,
		targetSize * GetStepSize());
	mSize = targetSize;
	UnlockThread();
	BIO_SANITIZE(mStore, ,
		return)
}

Index Container::Add(const ByteStream content)
{
	Index ret = GetNextAvailableIndex();
	BIO_SANITIZE(ret, ,
		return ret)
	LockThread();
	std::memcpy(
		&mStore[ret * sizeof(ByteStream)],
		content,
		sizeof(ByteStream));
	UnlockThread();
	return ret;
}

Index Container::Insert(
	const ByteStream content,
	const Index index
)
{
	BIO_SANITIZE(index, ,
		return InvalidIndex())

	if (index == mFirstFree)
	{
		//no adjustment necessary.
		return Add(content);
	}

	if (GetAllocatedSize() == GetCapacity())
	{
		Expand();
	}

	//adjust all deallocated positions.
	std::deque< Index > adjustedDeallocations;
	for (
		std::deque< Index >::iterator dlc = mDeallocated.begin();
		dlc != mDeallocated.end();
		++dlc
		)
	{
		adjustedDeallocations.push_front(*dlc + 1);
	}

	//move all memory down 1.
	LockThread();
	std::memcpy(
		&mStore[index * GetStepSize()],
		&mStore[(index + 1) * GetStepSize()],
		(mFirstFree - index) * GetStepSize());

	mDeallocated = adjustedDeallocations;
	mDeallocated.push_front(index); //make sure we add to the desired index.
	UnlockThread();

	//add the content.
	return Add(content);
}

ByteStream Container::Access(const Index index)
{
	BIO_SANITIZE(IsAllocated(index), ,
		return NULL)
	return *ForceCast< ByteStream* >(&mStore[index * sizeof(ByteStream)]);
}

const ByteStream Container::Access(const Index index) const
{
	BIO_SANITIZE(IsAllocated(index), ,
		return NULL)
	return *ForceCast< ByteStream* >(&mStore[index * sizeof(ByteStream)]);
}

Index Container::SeekTo(const ByteStream content) const
{
	Index ret = InvalidIndex();
	Iterator* itt = ConstructClassIterator();
	itt->MoveTo(GetEndIndex());
	for (
		; !itt->IsBeforeBeginning();
		itt->Decrement()
		)
	{
		if (AreEqual(
			itt->GetIndex(),
			content
		))
		{
			ret = itt->GetIndex();
			break;
		}
	}
	delete itt;
	return ret;
}

bool Container::Has(const ByteStream content) const
{
	return SeekTo(content); //implicit cast to bool should work.
}

ByteStream Container::Erase(const Index index)
{
	ByteStream ret;
	BIO_SANITIZE(this->IsAllocated(index), ,
		return ret)
	ret = Access(index);
	LockThread();
	this->mDeallocated.push_back(index);
	UnlockThread();
	return ret;
}

void Container::Import(const Container& other)
{
	for (
		SmartIterator otr = other.End();
		!otr.IsBeforeBeginning();
		--otr
		)
	{
		Add(*otr);
	}
}

void Container::Import(const Container* other)
{
	BIO_SANITIZE(other, ,
		return)
	Import(*other);
}

void Container::Clear()
{
	LockThread();
	mFirstFree = 1;
	mDeallocated.clear();
	UnlockThread();
}

Iterator* Container::ConstructClassIterator(const Index index) const
{
	Iterator* ret = new Iterator(
		this,
		index
	);
	return ret;
}

SmartIterator Container::Begin() const
{
	return SmartIterator(
		this,
		GetBeginIndex());
}

SmartIterator Container::End() const
{
	return SmartIterator(
		this,
		GetEndIndex());
}

ByteStream Container::operator[](const Index index)
{
	return Access(index);
}

const ByteStream Container::operator[](const Index index) const
{
	return Access(index);
}

ByteStream Container::operator[](const SmartIterator itt)
{
	return Access(itt);
}

const ByteStream Container::operator[](const SmartIterator itt) const
{
	return Access(itt);
}

Index Container::GetNextAvailableIndex()
{
	Index ret = InvalidIndex();
	if (!mDeallocated.empty())
	{
		LockThread();
		ret = mDeallocated.front();
		mDeallocated.pop_front();
		UnlockThread();
	}
	else if (GetAllocatedSize() == GetCapacity())
	{
		Expand();
		LockThread();
		ret = mFirstFree++;
		UnlockThread();
	}
	else
	{
		LockThread();
		ret = mFirstFree++;
		UnlockThread();
	}
	return ret;
}

bool Container::AreEqual(
	Index internal,
	const ByteStream external
) const
{
	return Access(internal) == external;
}

const ::std::size_t Container::GetStepSize() const
{
	return sizeof(ByteStream);
}

} //bio namespace
