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

#include "bio/common/container/Iterator.h"
#include "bio/common/container/Container.h"

namespace bio {

Iterator::Iterator(
	const Container* container,
	const Index index
)
	:
	mContainer(const_cast< Container* >(container)),
	mIndex(index)
{

}

Iterator::~Iterator()
{

}

Index Iterator::GetIndex() const
{
	return mIndex;
}

bool Iterator::MoveTo(const Index index)
{
	if (mContainer->IsAllocated(index))
	{
		mIndex = index;
		return true;
	}
	return false;
}

bool Iterator::IsAtBeginning() const
{
	return !mIndex;
}

bool Iterator::IsAtEnd() const
{
	return mIndex == mContainer->GetAllocatedSize();
}

Iterator* Iterator::Increment()
{
	if (mIndex >= mContainer->GetAllocatedSize())
	{
		mIndex = mContainer->GetAllocatedSize();
		return this;
	}
	while (mContainer->IsFree(++mIndex) && !IsAtEnd())
	{
		continue; //avoid re-referencing mIndex; see condition.
	}
	return this;
}

Iterator* Iterator::Decrement()
{
	if (!mIndex)
	{
		return this;
	}
	while (mContainer->IsFree(--mIndex) && !IsAtBeginning())
	{
		continue; //avoid re-referencing mIndex; see condition.
	}
	return this;
}

ByteStream Iterator::operator*()
{
	return mContainer->Access(mIndex);
}

const ByteStream Iterator::operator*() const
{
	return mContainer->Access(mIndex);
}

bool Iterator::IsValid() const
{
	return GetIndex() != InvalidIndex();
}

void Iterator::Invalidate()
{
	mIndex = InvalidIndex();
}
} //bio namespace
