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

#include "bio/physical/arrangement/SmartIterator.h"
#include "bio/physical/arrangement/Iterator.h"
#include "bio/physical/arrangement/Arrangement.h"

namespace bio {
namespace physical {

SmartIterator::SmartIterator(const Arrangement* arrangement)
	:
	m_implementation(arrangement->ConstructClassIterator(arrangement->GetEndIndex()))
{

}

SmartIterator::SmartIterator(
	const Arrangement* arrangement,
	Index index)
	:
	m_implementation(arrangement->ConstructClassIterator(index))
{

}

SmartIterator::~SmartIterator()
{
	delete m_implementation;
}

SmartIterator::operator Iterator*()
{
	return m_implementation;
}

Index SmartIterator::GetIndex() const
{
	return m_implementation->GetIndex();
}

bool SmartIterator::MoveTo(Index index) const
{
	return m_implementation->MoveTo(index);
}

bool SmartIterator::IsAtBeginning() const
{
	return m_implementation->IsAtBeginning();
}

bool SmartIterator::IsAtEnd() const
{
	return m_implementation->IsAtEnd();
}

ByteStream SmartIterator::operator*()
{
	return **m_implementation;
}

const ByteStream SmartIterator::operator*() const
{
	return **m_implementation;
}

SmartIterator& SmartIterator::operator++() const
{
	m_implementation->Increment();
	return *const_cast< SmartIterator* >(this);
}

SmartIterator SmartIterator::operator++(int) const
{
	SmartIterator ret = *this;
	m_implementation->Increment();
	return ret;
}

SmartIterator& SmartIterator::operator--() const
{
	m_implementation->Decrement();
	return *const_cast< SmartIterator* >(this);
}

SmartIterator SmartIterator::operator--(int) const
{
	SmartIterator ret = *this;
	m_implementation->Decrement();
	return ret;
}

} //physical namespace
} //bio namespace
