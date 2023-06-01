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

#include "bio/physical/symmetry/Symmetry.h"
#include "bio/physical/Time.h"
#include "bio/common/string/String.h"

namespace bio {
namespace physical {

Symmetry::Symmetry()
	:
	Class< Symmetry >(this),
	Identifiable< Id >(&SymmetryPerspective::Instance()),
	mType(&SymmetryTypePerspective::Instance()),
	mTimeCreated(GetCurrentTimestamp()),
	mTimeUpdated(0),
	mRealization(NULL)
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
		&SymmetryTypePerspective::Instance()
	),
	mTimeCreated(GetCurrentTimestamp()),
	mTimeUpdated(0),
	mRealization(NULL)
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
		&SymmetryTypePerspective::Instance()
	),
	mTimeCreated(GetCurrentTimestamp()),
	mTimeUpdated(0),
	mRealization(NULL)
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
		&SymmetryTypePerspective::Instance()
	),
	mTimeCreated(GetCurrentTimestamp()),
	mTimeUpdated(0),
	mRealization(NULL)
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
		&SymmetryTypePerspective::Instance()
	),
	mTimeCreated(GetCurrentTimestamp()),
	mTimeUpdated(0),
	mRealization(NULL)
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
	mTimeUpdated = GetCurrentTimestamp();
	mValue = bytes;
}

const ByteStream& Symmetry::GetValue() const
{
	return mValue;
}

ByteStream* Symmetry::AccessValue()
{
	mTimeUpdated = GetCurrentTimestamp();
	return &mValue;
}

const Timestamp& Symmetry::GetTimeCreated() const
{
	return mTimeCreated;
}

const Timestamp& Symmetry::GetTimeUpdated() const
{
	return mTimeUpdated;
}

void Symmetry::SetRealization(Wave* realization)
{
	mRealization = realization;
}

void Symmetry::Realize()
{
	BIO_SANITIZE(mRealization,,return)
	mRealization->Reify(this);
}

} //physical namespace
} //bio namespace
