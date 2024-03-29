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

#include "bio/chemical/symmetry/Symmetry.h"

namespace bio {
namespace chemical {

Symmetry::Symmetry(
	const Name& name,
	const Name& type
)
	:
	chemical::Class< Symmetry >(this),
	physical::Symmetry(
		name,
		type
	)
{

}

Symmetry::Symmetry(
	const Name& name,
	SymmetryType type
)
	:
	chemical::Class< Symmetry >(this),
	physical::Symmetry(
		name,
		type
	)
{

}

Symmetry::Symmetry(
	Id id,
	const Name& type
)
	:
	chemical::Class< Symmetry >(this),
	physical::Symmetry(
		id,
		type
	)
{

}

Symmetry::Symmetry(
	Id id,
	SymmetryType type
)
	:
	chemical::Class< Symmetry >(this),
	physical::Symmetry(
		id,
		type
	)
{

}

Symmetry::~Symmetry()
{

}

const Miscibility& Symmetry::GetMiscibility() const
{
	return mMiscibility;
}

void Symmetry::SetMiscibility(const Miscibility& miscibility)
{
	mMiscibility = miscibility;
}

void Symmetry::Recurse()
{
	Add< State >(state::Recursive());
}

void Symmetry::DoNotRecurse()
{
	Remove< State >(state::Recursive());
}

bool Symmetry::IsRecursive() const
{
	return Has< State >(state::Recursive());
}

} //chemical namespace
} //bio namespace
