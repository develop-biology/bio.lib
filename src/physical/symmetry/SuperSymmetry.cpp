/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "bio/physical/symmetry/SuperSymmetry.h"
#include "bio/physical/wave/Interference.h"
#include "bio/physical/common/SymmetryTypes.h"

namespace bio {
namespace physical {

SuperSymmetry::SuperSymmetry(
	const Id& symmetry,
	const Superposition& superposition,
	Interference* interference
)
	:
	Symmetry(symmetry, symmetry_type::Super()),
	mSuperposition(superposition),
	mInterference(interference)
{
}

SuperSymmetry::~SuperSymmetry()
{
	if (mInterference != NULL)
	{
		delete mInterference;
	}
	mInterference = NULL;
}

const Superposition& SuperSymmetry::GetSuperposition() const
{
	return mSuperposition;
}

void SuperSymmetry::SetSuperposition(const Superposition& superposition)
{
	mSuperposition = superposition;
}

const Interference* SuperSymmetry::GetInterference() const
{
	return mInterference;
}

void SuperSymmetry::SetInterference(Interference* interference)
{
	mInterference = interference;
}

} // namespace physical
} // namespace bio
