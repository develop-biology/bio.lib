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
	m_container(const_cast< Container* >(container)), 
	m_index(index)
{

}

Iterator::~Iterator()
{

}

Index Iterator::GetIndex() const
{
	return m_index;
}

bool Iterator::MoveTo(const Index index)
{
	if (m_container->IsAllocated(index))
	{
		m_index = index;
		return true;
	}
	return false;
}

bool Iterator::IsAtBeginning() const
{
	return !m_index;
}

bool Iterator::IsAtEnd() const
{
	return m_index == m_container->GetAllocatedSize();
}

Iterator* Iterator::Increment() 
{
	if (m_index >= m_container->GetAllocatedSize())
	{
		m_index = m_container->GetAllocatedSize();
		return this;
	}
	while (m_container->IsFree(++m_index) && !IsAtEnd())
	{
		continue; //avoid re-referencing m_index; see condition.
	}
	return this;
}

Iterator* Iterator::Decrement() 
{
	if (!m_index)
	{
		return this;
	}
	while (m_container->IsFree(--m_index) && !IsAtBeginning())
	{
		continue; //avoid re-referencing m_index; see condition.
	}
	return this;
}

ByteStream Iterator::operator*()
{
	return m_container->Access(m_index);
}

const ByteStream Iterator::operator*() const
{
	return m_container->Access(m_index);
}

} //bio namespace
