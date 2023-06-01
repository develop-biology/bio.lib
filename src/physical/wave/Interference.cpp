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

#include "bio/physical/wave/Interference.h"
#include "bio/physical/common/Superpositions.h"
#include "bio/physical/symmetry/SuperSymmetry.h"

namespace bio {
namespace physical {

Interference::Interference():
	physical::Class< Interference >(this, NULL)
{
}

Interference::~Interference()
{
}

Superposition Interference::GetSuperpositionFor(const Id &symmetry) const
{
    const SuperSymmetry* superSymmetry = GetSuperSymmetryFor(symmetry);
	BIO_SANITIZE(superSymmetry, , return SuperpositionPerspective::InvalidId())
	return superSymmetry->GetSuperposition();
}

void Interference::SetSuperpositionFor(const Id &symmetry, const Superposition &superposition)
{
	Index index = mSuperSymmetries.SeekToId(symmetry);
	if (!index)
	{
		mSuperSymmetries.Add(Linear(new SuperSymmetry(symmetry, superposition)));
	}
	SuperSymmetry* superSymmetry = GetSuperSymmetryFor(symmetry);
	BIO_SANITIZE(superSymmetry, , return NULL)
	superSymmetry->SetSuperposition(superposition);
}

const Interference* Interference::GetInterferenceFor(const Id& symmetry) const
{
	const SuperSymmetry* superSymmetry = GetSuperSymmetryFor(symmetry);
	BIO_SANITIZE(superSymmetry, , return NULL)
	return superSymmetry->GetInterference();
}

void Interference::SetInterferenceFor(const Id &symmetry, Interference *interference)
{
	Index index = mSuperSymmetries.SeekToId(symmetry);
	if (!index)
	{
		mSuperSymmetries.Add(Linear(new SuperSymmetry(symmetry, superposition::Complex(), interference)));
	}
	SuperSymmetry* superSymmetry = GetSuperSymmetryFor(symmetry);
	BIO_SANITIZE(superSymmetry, , return NULL)
	superSymmetry->SetInterference(interference);
}

const SuperSymmetry* Interference::GetSuperSymmetryFor(const Id& symmetry) const
{
	Index index = mSuperSymmetries.SeekToId(symmetry);
	BIO_SANITIZE(index, , return NULL)
	return ForceCast< const SuperSymmetry* >(mSuperSymmetries.LinearAccess(index));
}

SuperSymmetry* Interference::GetSuperSymmetryFor(const Id& symmetry)
{
	Index index = mSuperSymmetries.SeekToId(symmetry);
	BIO_SANITIZE(index, , return NULL)
	return ForceCast< SuperSymmetry* >(mSuperSymmetries.LinearAccess(index));
}

} //physical namespace
} //bio namespace
