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

#include "bio/common/container/SmartIterator.h"
#include "bio/common/container/Iterator.h"
#include "bio/common/container/Container.h"

namespace bio {

SmartIterator::SmartIterator(const Container* container)
	:
	mImplementation(container->ConstructClassIterator(container->GetEndIndex()))
{

}

SmartIterator::SmartIterator(
	const Container* container,
	Index index
)
	:
	mImplementation(container->ConstructClassIterator(index))
{

}

SmartIterator::~SmartIterator()
{
	delete mImplementation;
}

bool SmartIterator::IsValid() const
{
	return mImplementation->IsValid();
}

void SmartIterator::Invalidate()
{
	mImplementation->Invalidate();
}

Iterator* SmartIterator::GetImplementation()
{
	return mImplementation;
}

const Iterator* SmartIterator::GetImplementation() const
{
	return mImplementation;
}

Index SmartIterator::GetIndex() const
{
	return mImplementation->GetIndex();
}

bool SmartIterator::MoveTo(Index index) const
{
	return mImplementation->MoveTo(index);
}

bool SmartIterator::IsBeforeBeginning() const
{
	return mImplementation->IsBeforeBeginning();
}

bool SmartIterator::IsAfterEnd() const
{
	return mImplementation->IsAfterEnd();
}

ByteStream SmartIterator::operator*()
{
	return **mImplementation;
}

const ByteStream SmartIterator::operator*() const
{
	return **mImplementation;
}

SmartIterator& SmartIterator::operator++() const
{
	mImplementation->Increment();
	return *const_cast< SmartIterator* >(this);
}

SmartIterator SmartIterator::operator++(int) const
{
	SmartIterator ret = *this;
	mImplementation->Increment();
	return ret;
}

SmartIterator& SmartIterator::operator--() const
{
	mImplementation->Decrement();
	return *const_cast< SmartIterator* >(this);
}

SmartIterator SmartIterator::operator--(int) const
{
	SmartIterator ret = *this;
	mImplementation->Decrement();
	return ret;
}

} //bio namespace
