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

#include "bio/molecular/Vesicle.h"
#include "bio/molecular/common/Filters.h"

namespace bio {
namespace molecular {

Vesicle::Vesicle(const Vesicle& toCopy)
	:
	molecular::Class< Vesicle >(
		this,
		toCopy.GetId(),
		toCopy.GetPerspective(),
		toCopy.GetFilter()),
	LinearMotif< Molecule* >(toCopy)
{
	LinearMotif< Molecule* >::mPerspective = this;
}

Vesicle::~Vesicle()
{
}

Molecule* Vesicle::operator[](Id moleculeId)
{
	return GetById< Molecule* >(moleculeId);
}

const Molecule* Vesicle::operator[](Id moleculeId) const
{
	return GetById< Molecule* >(moleculeId);
}

Molecule* Vesicle::operator[](const Name& moleculeName)
{
	return GetByName< Molecule* >(moleculeName);
}

const Molecule* Vesicle::operator[](const Name& moleculeName) const
{
	return GetByName< Molecule* >(moleculeName);
}

} //molecular namespace
} //bio namespace
