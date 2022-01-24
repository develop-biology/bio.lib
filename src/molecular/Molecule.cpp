/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/molecular/Molecule.h"
#include "bio/molecular/BondTypes.h"
#include "bio/molecular/Filters.h"
#include "bio/chemical/BondTypes.h"

namespace bio {
namespace molecular {

Molecule::Molecule() :
	Class(this),
	physical::Perspective<StandardDimension>(),
	chemical::LinearStructuralComponent<Surface*>(this)
{

}

Molecule::Molecule(Name name)
	:
	Class(this, name, &MoleculePerspective::Instance(), filter::Molecular()),
	physical::Perspective<StandardDimension>(),
	chemical::LinearStructuralComponent<Surface*>(this)
{

}

Molecule::Molecule(StandardDimension id)
	:
	Class(this, name, &MoleculePerspective::Instance(), filter::Molecular()),
	physical::Perspective<StandardDimension>(),
	chemical::LinearStructuralComponent<Surface*>(this)
{

}

Molecule::Molecule(const Molecule& toCopy)
	:
	Class(this, toCopy.GetId(), toCopy().GetPerspective(), toCopy().GetFilter()),
	physical::Perspective<StandardDimension>(toCopy),
	chemical::LinearStructuralComponent<Surface*>(toCopy)
{
	chemical::LinearStructuralComponent<Surface*>::m_perspective = this;
}

Molecule::~Molecule()
{
}

Surface* Molecule::RotateTo(StandardDimension surfaceId)
{
	return GetById<Surface*>(surfaceId, false);
}

const Surface* Molecule::RotateTo(StandardDimension surfaceId) const
{
	return GetById<Surface*>(surfaceId, false);
}

Surface* Molecule::RotateTo(Name surfaceName)
{
	return GetByName<Surface*>(surfaceName, false);
}

const Surface* Molecule::RotateTo(Name surfaceName) const
{
	return GetByName<Surface*>(surfaceName, false);
}

bool Molecule::DuplicateFrom(Molecule* source, Name surface)
{
	BIO_SANITIZE(source,,return false);
	BIO_SANITIZE(!RotateTo(surface),,return false);
	Surface* toTransfer = NULL;
	BIO_SANITIZE_WITH_CACHE(source->RotateTo(surface), toTransfer=RESULT, return false);
	Add<Surface*>(toTransfer->Clone())->SetEnvironment(this);
	return true;
}
bool Molecule::TransferFrom(Molecule,* source, Name surface)
{
	BIO_SANITIZE(source,,return false);
	BIO_SANITIZE(!RotateTo(surface),,return false);
	Surface* toTransfer = NULL;
	BIO_SANITIZE_WITH_CACHE(source->RotateTo(surface), toTransfer=RESULT, return false);
	Add<Surface*>(toTransfer)->SetEnvironment(this);
	source->Remove<Surface*>(toTransfer);
	return true;
}

Surface* Molecule::operator[](StandardDimension surfaceId)
{
	return RotateTo(surfaceId);
}
const Surface* Molecule::operator[](StandardDimension surfaceId) const
{
	return RotateTo(surfaceId);
}
Surface* Molecule::operator[](Name name)
{
	return RotateTo(name);
}
const Surface* Molecule::operator[](Name name) const
{
	return RotateTo(name);
}

Molecule* Molecule::operator<<(Surface* source)
{
	BIO_SANITIZE(source,, return this);
	Add<Surface*>(souce->Clone())->SetEnvironment(this);
	return this;
}

Surface* Molecule::operator>>(Surface* target)
{
	BIO_SANITIZE(target,, return target);
	target->Add<Molecule*>(this->Clone());
	return target;
}

Molecule* Molecule::operator<<=(Surface* source)
{
	BIO_SANITIZE(source,, return this);
	Add<Surface*>(souce)->SetEnvironment(this);
	return this;
}

Surface* Molecule::operator>>=(Surface* target)
{
	BIO_SANITIZE(target,, return target);
	target->Add<Molecule*>(this);
	return target;
}

Molecule* Molecule::operator<<(Molecule* source)
{
	BIO_SANITIZE(source,, return this);
	Import<Surface*>(source);
	return this;
}

Molecule* Molecule::operator>>(Molecule* target)
{
	BIO_SANITIZE(target,, return target);
	target->Import<Surface*>(this);
	return target;
}

Molecule* Molecule::operator<<=(Molecule* source)
{
	BIO_SANITIZE(source,, return this);
	LockThread();
	AbstractStructuralComponent<Surface*>::m_contents.insert(AbstractStructuralComponent<Surface*>::m_contents.end(),source::AbstractStructuralComponent<Surface*>::m_contents.begin(), source::AbstractStructuralComponent<Surface*>::m_contents.end());
	source::AbstractStructuralComponent<Surface*>::m_contents.clear();
	UnlockThread();
	return this;
}

Molecule* Molecule::operator>>=(Molecule* target)
{
	BIO_SANITIZE(target,,return target);
	target <<= this;
	return target;
}

} //molecular namespace
} //bio namespace
