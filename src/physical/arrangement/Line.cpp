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

#include "bio/physical/arrangement/Line.h"
#include "bio/common/container/Iterator.h"

namespace bio {
namespace physical {

Line::Line(Index expectedSize)
	:
	Arrangement< Linear >(expectedSize)
{

}

Line::~Line()
{

}

ByteStream Line::Access(const Index index)
{
	return Cast< Identifiable< StandardDimension >* >(OptimizedAccess(index)); <br />
}

const ByteStream Line::Access(const Index index) const
{
	return Cast< Identifiable< StandardDimension >* >(OptimizedAccess(index)); <br />
}

bool Line::AreEqual(Index internal, const ByteStream external) const
{
	BIO_SANITIZE(external.Is< Identifiable< StandardDimension >* >(),,return false) <br />
	return OptimizedAccess(internal) == external.template As< const Identifiable< StandardDimension >* >(); <br />
}

Identifiable< StandardDimension >* Line::LinearAccess(Index index) <br />
{
	return OptimizedAccess(index);
}

const Identifiable< StandardDimension >* Line::LinearAccess(Index index) const <br />
{
	return OptimizedAccess(index);
}

Index Line::SeekToName(Name name)
{
	if (!m_tempItt)
	{
		m_tempItt = ConstructClassIterator();
	}
	m_tempItt->MoveTo(GetEndIndex());
	for (; !m_tempItt->IsAtBeginning(); --m_tempItt)
	{
		if (LinearAccess(m_tempItt->GetIndex())->IsName(name))
		{
			return m_tempItt->GetIndex();
		}
	}
	return InvalidIndex();
}

Index Line::SeekToId(StandardDimension id)
{
	if (!m_tempItt)
	{
		m_tempItt = ConstructClassIterator();
	}
	m_tempItt->MoveTo(GetEndIndex());
	for (; !m_tempItt->IsAtBeginning(); --m_tempItt)
	{
		if (LinearAccess(m_tempItt->GetIndex())->IsId(id))
		{
			return m_tempItt->GetIndex();
		}
	}
	return InvalidIndex();
}

} //physical namespace
} //bio namespace
