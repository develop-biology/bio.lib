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

#include "bio/molecular/channel/TransMembraneDomain.h"

namespace bio {
namespace molecular {

TransMembraneDomain::TransMembraneDomain(Vesicle* inner) :
	mInner(inner)
{
	
}


TransMembraneDomain::~TransMembraneDomain()
{
	
}

void TransMembraneDomain::SetVesicle(Vesicle* inner)
{
	mInner = inner;
}


Vesicle* TransMembraneDomain::GetVesicle()
{
	return mInner;
}


const Vesicle* TransMembraneDomain::GetVesicle() const
{
	return mInner;
}

	
void TransMembraneDomain::Ingress(Molecule* outer)
{
	BIO_SANITIZE(mInner,,return)
	BIO_SANITIZE(outer,,return)
	mInner->Add< chemical::Solute* >(outer);
}

	
void TransMembraneDomain::Ingress(Vesicle* outer)
{
	BIO_SANITIZE(mInner,,return)
	BIO_SANITIZE(outer,,return)
	mInner->Import< chemical::Solute* >(outer->GetAll< chemical::Solute* >());
}

	
Molecule* TransMembraneDomain::Egress(const Name& moleculeName)
{
	BIO_SANITIZE(mInner,,return NULL)
	Molecule* found = mInner->GetByName< chemical::Solute* >(moleculeName);
	BIO_SANITIZE(found,,return NULL)
	return CloneAndCast(found);
}


const Molecule* TransMembraneDomain::Egress(const Name& moleculeName) const
{
	BIO_SANITIZE(mInner,,return NULL)
	chemical::Solute* found = mInner->GetByName< chemical::Solute* >(moleculeName);
	BIO_SANITIZE(found,,return NULL)
	return CloneAndCast(found);
}

	
Molecule* TransMembraneDomain::Egress(const Id& moleculeId)
{
	BIO_SANITIZE(mInner,,return NULL)
	chemical::Solute* found = mInner->GetById< chemical::Solute* >(moleculeId);
	BIO_SANITIZE(found,,return NULL)
	return CloneAndCast(found);
}

	
const Molecule* TransMembraneDomain::Egress(const Id& moleculeId) const
{
	BIO_SANITIZE(mInner,,return NULL)
	chemical::Solute* found = mInner->GetById< chemical::Solute* >(moleculeId);
	BIO_SANITIZE(found,,return NULL)
	return CloneAndCast(found);
}


Molecule* TransMembraneDomain::Secrete(const Name& moleculeName)
{
	BIO_SANITIZE(mInner,,return NULL)
	physical::Line* molecules = Cast< physical::Line* >(mInner->GetAll< chemical::Solute* >());
	Index found = molecules->SeekToName(moleculeName);
	BIO_SANITIZE(found,,return NULL)
	return ChemicalCast< Molecule* >(molecules->Erase(found).As< physical::Linear >().operator physical::Identifiable< Id >*());
}


Molecule* TransMembraneDomain::Secrete(const Id& moleculeId)
{
	BIO_SANITIZE(mInner,,return NULL)
	physical::Line* molecules = Cast< physical::Line* >(mInner->GetAll< chemical::Solute* >());
	Index found = molecules->SeekToId(moleculeId);
	BIO_SANITIZE(found,,return NULL)
	return ChemicalCast< Molecule* >(molecules->Erase(found).As< physical::Linear >().operator physical::Identifiable< Id >*());
}


} //molecular namespace
} //bio namespace
