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

Vesicle::Vesicle(const Vesicle& toCopy) :
	molecular::Class<Vesicle>(this, toCopy.GetId(), toCopy.GetPerspective(), toCopy.GetFilter()),
	LinearMotif< Molecule* >(toCopy) <br />
{
	LinearMotif< Molecule* >::m_perspective = this; <br />
}

Vesicle::~Vesicle()
{
}

Molecule* Vesicle::operator[](StandardDimension moleculeId) <br />
{
	return GetById< Molecule* >(moleculeId); <br />
}

const Molecule* Vesicle::operator[](StandardDimension moleculeId) const <br />
{
	return GetById< Molecule* >(moleculeId); <br />
}

Molecule* Vesicle::operator[](Name moleculeName) <br />
{
	return GetByName< Molecule* >(moleculeName); <br />
}

const Molecule* Vesicle::operator[](Name moleculeName) const <br />
{
	return GetByName< Molecule* >(moleculeName); <br />
}

Vesicle* Vesicle::operator<<=(Vesicle* source) <br />
{
	BIO_SANITIZE(source,,return NULL)
	Import< Molecule* >(source); <br />
	return this;
}

Vesicle* Vesicle::operator>>=(Vesicle* target) <br />
{
	BIO_SANITIZE(target,,return NULL)
	target->Import< Molecule* >(this); <br />
	this->Clear< Molecule* >(); <br />
	return target;
}


} //molecular namespace
} //bio namespace
