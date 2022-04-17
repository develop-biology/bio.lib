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
	physical::Perspective< StandardDimension >(toCopy),
	chemical::LinearMotif< Surface* >(toCopy) <br />
{
	chemical::LinearMotif< Surface* >::m_perspective = this; <br />
}

Molecule::~Molecule()
{
}

Surface* Molecule::RotateTo(StandardDimension surfaceId) <br />
{
	BIO_SANITIZE_WITH_CACHE(GetById< Surface* >( <br />
		surfaceId
	),
		return *Cast< Surface** >(RESULT), <br />
		return NULL);
}

const Surface* Molecule::RotateTo(StandardDimension surfaceId) const <br />
{
	BIO_SANITIZE_WITH_CACHE(GetById< Surface* >( <br />
		surfaceId
	),
		return *Cast< const Surface** >(RESULT), <br />
		return NULL);
}

Surface* Molecule::RotateTo(Name surfaceName) <br />
{
	BIO_SANITIZE_WITH_CACHE(GetByName< Surface* >( <br />
		surfaceName
	),
		return *Cast< Surface** >(RESULT), <br />
		return NULL);
}

const Surface* Molecule::RotateTo(Name surfaceName) const <br />
{
	BIO_SANITIZE_WITH_CACHE(GetByName< Surface* >( <br />
		surfaceName
	),
		return *Cast< const Surface** >(RESULT), <br />
		return NULL);
}

bool Molecule::DuplicateFrom(
	Molecule* source, <br />
	Name surface
)
{
	BIO_SANITIZE(source, ,
		return false);

	BIO_SANITIZE(!RotateTo(surface), ,
		return false);

	Surface* toTransfer = NULL; <br />

	BIO_SANITIZE_WITH_CACHE(source->RotateTo(surface),
		toTransfer = RESULT,
		return false);

	Add< Surface* >(CloneAndCast< Surface* >(toTransfer))->SetEnvironment(this); <br />
	return true;
}

bool Molecule::TransferFrom(
	Molecule* source, <br />
	Name surface
)
{
	BIO_SANITIZE(source, ,
		return false);

	BIO_SANITIZE(!RotateTo(surface), ,
		return false);

	Surface* toTransfer = NULL; <br />

	BIO_SANITIZE_WITH_CACHE(source->RotateTo(surface),
		toTransfer = RESULT,
		return false);

	Add< Surface* >(toTransfer)->SetEnvironment(this); <br />
	source->Remove< Surface* >(toTransfer); <br />
	return true;
}

Surface* Molecule::operator()(StandardDimension surfaceId) <br />
{
	return RotateTo(surfaceId);
}

const Surface* Molecule::operator()(StandardDimension surfaceId) const <br />
{
	return RotateTo(surfaceId);
}

Surface* Molecule::operator()(Name name) <br />
{
	return RotateTo(name);
}

const Surface* Molecule::operator()(Name name) const <br />
{
	return RotateTo(name);
}

Molecule* Molecule::operator<<(Surface* source) <br />
{
	BIO_SANITIZE(source, ,
		return this);
	Add< Surface* >(source)->SetEnvironment(this); <br />
	return this;
}

Surface* Molecule::operator>>(Surface* target) <br />
{
	BIO_SANITIZE(target, ,
		return target);
	target->Add< Molecule* >(this); <br />
	return target;
}

Molecule* Molecule::operator<<(Molecule* source) <br />
{
	BIO_SANITIZE(source, ,
		return this);
	Import< Surface* >(source); <br />
	return this;
}

Molecule* Molecule::operator>>(Molecule* target) <br />
{
	BIO_SANITIZE(target, ,
		return target);
	target->Import< Surface* >(this); <br />
	Clear< Surface* >(); <br />
	return target;
}

physical::Symmetry* Molecule::Spin() const <br />
{
	//TODO...
	return NULL;
}

Code Molecule::Reify(physical::Symmetry* symmetry) <br />
{
	//TODO...
	return code::NotImplemented();
}

} //molecular namespace
} //bio namespace
