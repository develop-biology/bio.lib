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

#include "bio/physical/Symmetry.h"
#include "bio/common/String.h"

namespace bio {
namespace physical {

Symmetry::Symmetry()
	:
	Class< Symmetry >(this),
	Identifiable< Id >(&SymmetryPerspective::Instance()),
	mType(&SymmetryTypePerspective::Instance())
{

}

Symmetry::Symmetry(
	const Name& name,
	const Name& type
)
	:
	Class< Symmetry >(this),
	mType(
		type,
		&SymmetryTypePerspective::Instance())
{
	Identifiable< Id >::Initialize(
		name,
		&SymmetryPerspective::Instance());
}

Symmetry::Symmetry(
	const Name& name,
	SymmetryType type
)
	:
	Class< Symmetry >(this),
	mType(
		type,
		&SymmetryTypePerspective::Instance())
{
	Identifiable< Id >::Initialize(
		name,
		&SymmetryPerspective::Instance());
}

Symmetry::Symmetry(
	Id id,
	const Name& type
)
	:
	Class< Symmetry >(this),
	mType(
		type,
		&SymmetryTypePerspective::Instance())
{
	Identifiable< Id >::Initialize(
		id,
		&SymmetryPerspective::Instance());
}

Symmetry::Symmetry(
	Id id,
	SymmetryType type
)
	:
	Class< Symmetry >(this),
	mType(
		type,
		&SymmetryTypePerspective::Instance())
{
	Identifiable< Id >::Initialize(
		id,
		&SymmetryPerspective::Instance());
}

Symmetry::~Symmetry()
{
}

const Identifiable< SymmetryType >& Symmetry::GetType() const
{
	return mType;
}

void Symmetry::SetType(SymmetryType type)
{
	mType.SetId(type);
}

void Symmetry::SetType(const Name& type)
{
	mType.SetName(type);
}

void Symmetry::SetValue(const ByteStream& bytes)
{
	mValue = bytes;
}

const ByteStream& Symmetry::GetValue() const
{
	return mValue;
}

ByteStream* Symmetry::AccessValue()
{
	return &mValue;
}

} //physical namespace
} //bio namespace
