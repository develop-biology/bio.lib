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

#include "bio/chemical/arrangement/Line.h"

namespace bio {
namespace chemical {

Line::Line(Position expectedSize)
	:
	Arrangement< Linear >(expectedSize)
{

}

Line::~Line()
{

}

Line::Position Line::Add(physical::Identifiable< StandardDimension >* content)
{
	Arrangement< Linear >::Add(Linear(content));
}

void Line::Import(const Line* other)
{
	BIO_SANITIZE(other, ,
		return)

	Iterator* otr = other->End();
	for (
		; !otr->IsAtBeginning();
		--otr
		)
	{
		Arrangement< Linear >::Add(Linear((*otr)->Clone()));
	}
}

physical::Identifiable< StandardDimension >* Line::Access(Position position)
{
	BIO_SANITIZE(this->IsValid(position), ,
		return NULL)
	return this->m_store[position]; //implicit user-defined cast.
}

const physical::Identifiable< StandardDimension >* Line::Access(Position position) const
{
	BIO_SANITIZE(this->IsValid(position), ,
		return NULL)
	return this->m_store[position]; //implicit user-defined cast.
}

Line::Position Line::SeekTo(const physical::Identifiable< StandardDimension >* content) const
{
	return Arrangement< Linear >::SeekTo(Linear(content));
}

bool Line::Has(const physical::Identifiable< StandardDimension >* content) const
{
	return SeekTo(content); //implicit cast to bool should work.
}

const physical::Identifiable< StandardDimension >* Line::GetInternalPointer(const physical::Identifiable< StandardDimension >* content) const
{
	return Access(SeekTo(content));
}

physical::Identifiable< StandardDimension >* Line::operator[](Position position)
{
	return Access(position);
}

const physical::Identifiable< StandardDimension >* Line::operator[](Position position) const
{
	return Access(position);
}


Line::Iterator::Iterator(
	const Line* line,
	Position position = InvalidPosition())
	:
	Arrangement< Linear >::Iterator(
		Line,
		position
	)
{

}

Line::Iterator::~Iterator()
{

}

physical::Identifiable< StandardDimension >* Line::Iterator::operator*()
{
	return Cast< Line* >(m_arrangement)->Access(m_position);
}

const physical::Identifiable< StandardDimension >* Line::Iterator::operator*() const
{
	return *Cast< Line* >(m_arrangement)->Access(m_position);
}

physical::Identifiable< StandardDimension >* Line::Iterator::operator->()
{
	return Cast< Line* >(m_arrangement)->Access(m_position);
}

const physical::Identifiable< StandardDimension >* Line::Iterator::operator->() const
{
	return Cast< Line* >(m_arrangement)->Access(m_position);
}

AbstractArrangement::Iterator* Line::ConstructClassIterator(Position position = InvalidPosition())
{
	BIO_SANITIZE(IsValid(position), ,
		return NULL)
	return new Iterator(
		this,
		position
	);
}

} //chemical namespace
} //bio namespace
