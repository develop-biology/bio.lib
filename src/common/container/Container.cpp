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
	mSize(expectedSize + 1),
	mTempItt(NULL)
{
	mStore = (unsigned char*)std::malloc(mSize * stepSize);
	BIO_ASSERT(mStore)
}

Container::Container(const Container& other)
	:
	mFirstFree(other.mFirstFree),
	mSize(other.mSize),
	mTempItt(NULL)
{
	mStore = (unsigned char*)std::malloc(mSize * other.GetStepSize());
	BIO_ASSERT(mStore)
	Import(other); // <- NOT VIRTUAL (in ctor).
}

Container::Container(const Container* other)
	:
	mFirstFree(other->mFirstFree),
	mSize(other->mSize),
	mTempItt(NULL)
{
	mStore = (unsigned char*)std::malloc(mSize * other->GetStepSize());
	BIO_ASSERT(mStore)
	Import(other); // <- NOT VIRTUAL (in ctor).
}

Container::~Container()
{
	Clear(); // <- NOT VIRTUAL (in dtor).
	if (mTempItt)
	{
		delete mTempItt;
		mTempItt = NULL;
	}
	std::free(mStore);
}

Index Container::GetBeginIndex() const
{
	return 1;
}

Index Container::GetEndIndex() const
{
	return GetAllocatedSize();
}

Index Container::GetCapacity() const
{
	return mSize;
}

Index Container::GetAllocatedSize() const
{
	return mFirstFree;
}

Index Container::GetNumberOfElements() const
{
	return GetAllocatedSize() - mDeallocated.size();
}

bool Container::IsInRange(const Index index) const
{
	return index && index < mSize;
}

bool Container::IsFree(Index index) const
{
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
	BIO_SANITIZE(mSize < ::std::numeric_limits< Index >::max(), ,
		return)
	Index targetSize = mSize * mSize; //squared. 
	if (targetSize < mSize)
	{
		targetSize = ::std::numeric_limits< Index >::max();
	}
	mStore = (unsigned char*)std::realloc(
		mStore,
		targetSize * GetStepSize());
	BIO_SANITIZE(mStore, ,
		return)
	mSize = targetSize;
}

Index Container::Add(const ByteStream content)
{
	Index ret = GetNextAvailableIndex();
	BIO_SANITIZE(ret, ,
		return ret)
	std::memcpy(
		&mStore[ret * sizeof(ByteStream)],
		content,
		sizeof(ByteStream));
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
		Add(content);
	}

	if (GetAllocatedSize() == GetCapacity())
	{
		Expand();
	}

	//move all memory down 1.
	std::memcpy(
		&mStore[index * GetStepSize()],
		&mStore[(index + 1) * GetStepSize()],
		(mFirstFree - index) * GetStepSize());

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
	mDeallocated = adjustedDeallocations;

	//make sure we add to the desired index.
	mDeallocated.push_front(index);

	//add the content.
	return Add(content);
}

ByteStream Container::Access(const Index index)
{
	BIO_SANITIZE(IsAllocated(index), ,
		return NULL)
	ByteStream* ret;
	std::memcpy(
		ret,
		&mStore[index * sizeof(ByteStream)],
		sizeof(ByteStream));
	return *ret;
}

const ByteStream Container::Access(const Index index) const
{
	BIO_SANITIZE(IsAllocated(index), ,
		return NULL)
	ByteStream* ret;
	std::memcpy(
		ret,
		&mStore[index * sizeof(ByteStream)],
		sizeof(ByteStream));
	return *ret;
}

Index Container::SeekTo(const ByteStream content) const
{
	if (!mTempItt)
	{
		mTempItt = ConstructClassIterator();
	}
	mTempItt->MoveTo(GetEndIndex());
	for (
		; !mTempItt->IsAtBeginning();
		--mTempItt
		)
	{
		if (AreEqual(
			mTempItt->GetIndex(),
			content
		))
		{
			return mTempItt->GetIndex();
		}
	}
	return InvalidIndex();
}

bool Container::Has(const ByteStream content) const
{
	return SeekTo(content); //implicit cast to bool should work.
}

bool Container::Erase(const Index index)
{
	BIO_SANITIZE(IsAllocated(index), ,
		return false)
	ByteStream* toDelete;
	std::memcpy(
		toDelete,
		&mStore[index * sizeof(ByteStream)],
		sizeof(ByteStream));
	delete toDelete;
	mDeallocated.push_back(index);
	return true;
}

void Container::Import(const Container& other)
{
	for (
		SmartIterator otr = other.End();
		!otr.IsAtBeginning();
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
	//Call destructors before clearing the array.
	if (!mTempItt)
	{
		mTempItt = ConstructClassIterator();
	}
	mTempItt->MoveTo(GetEndIndex());
	for (
		; !mTempItt->IsAtBeginning();
		--mTempItt
		)
	{
		Erase(mTempItt->GetIndex());
	}

	mFirstFree = 1;
	mDeallocated.clear();
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
		ret = mDeallocated.front();
		mDeallocated.pop_front();
	}
	else if (mFirstFree == mSize)
	{
		Expand();
		ret = mFirstFree++;
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
