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

#include "bio/molecular/Molecule.h"
#include "bio/molecular/Protein.h"
#include "bio/molecular/common/BondTypes.h"
#include "bio/molecular/common/Filters.h"
#include "bio/chemical/common/BondTypes.h"

namespace bio {
namespace molecular {

Molecule::Molecule(const Molecule& toCopy)
	:
	molecular::Class< Molecule >(
		this,
		toCopy.GetId(),
		toCopy.GetPerspective(),
		toCopy.GetFilter()),
	physical::Perspective< Id >(toCopy),
	chemical::LinearMotif< Surface* >(toCopy)
{
	chemical::LinearMotif< Surface* >::mPerspective = this;
}

Molecule::~Molecule()
{
}

Surface* Molecule::RotateTo(Id surfaceId)
{
	return GetById< Surface* >(surfaceId);
}

const Surface* Molecule::RotateTo(Id surfaceId) const
{
	return GetById< Surface* >(surfaceId);
}

Surface* Molecule::RotateTo(const Name& surfaceName)
{
	return GetByName< Surface* >(surfaceName);
}

const Surface* Molecule::RotateTo(const Name& surfaceName) const
{
	return GetByName< Surface* >(surfaceName);
}

bool Molecule::DuplicateFrom(
	Molecule* source,
	const Name& surface
)
{
	BIO_SANITIZE(source, ,
		return false);

	BIO_SANITIZE(!RotateTo(surface), ,
		return false);

	Surface* toTransfer = NULL;

	BIO_SANITIZE_WITH_CACHE(source->RotateTo(surface),
		toTransfer = RESULT,
		return false);

	Add< Surface* >(CloneAndCast< Surface* >(toTransfer))->SetEnvironment(this);
	return true;
}

bool Molecule::TransferFrom(
	Molecule* source,
	const Name& surface
)
{
	BIO_SANITIZE(source, ,
		return false);

	BIO_SANITIZE(!RotateTo(surface), ,
		return false);

	Surface* toTransfer = NULL;

	BIO_SANITIZE_WITH_CACHE(source->RotateTo(surface),
		toTransfer = RESULT,
		return false);

	Add< Surface* >(toTransfer)->SetEnvironment(this);
	source->Remove< Surface* >(toTransfer);
	return true;
}

Surface* Molecule::operator()(Id surfaceId)
{
	return RotateTo(surfaceId);
}

const Surface* Molecule::operator()(Id surfaceId) const
{
	return RotateTo(surfaceId);
}

Surface* Molecule::operator()(const Name& name)
{
	return RotateTo(name);
}

const Surface* Molecule::operator()(const Name& name) const
{
	return RotateTo(name);
}

Molecule* Molecule::operator<<(Surface* source)
{
	BIO_SANITIZE(source, ,
		return this);
	Add< Surface* >(source)->SetEnvironment(this);
	return this;
}

Surface* Molecule::operator>>(Surface* target)
{
	BIO_SANITIZE(target, ,
		return target);
	target->Add< Molecule* >(this);
	return target;
}

Molecule* Molecule::operator<<(Molecule* source)
{
	BIO_SANITIZE(source, ,
		return this);
	Import< Surface* >(source);
	return this;
}

Molecule* Molecule::operator>>(Molecule* target)
{
	BIO_SANITIZE(target, ,
		return target);
	target->Import< Surface* >(this);
	Clear< Surface* >();
	return target;
}

physical::Symmetry* Molecule::Spin() const
{
	//TODO...
	return NULL;
}

Code Molecule::Reify(physical::Symmetry* symmetry)
{
	//TODO...
	return code::NotImplemented();
}

} //molecular namespace
} //bio namespace
