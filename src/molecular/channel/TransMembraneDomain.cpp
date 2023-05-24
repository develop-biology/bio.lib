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

TransMembraneDomain::TransMembraneDomain(Vesicle* interior) :
	mInterior(interior)
{
	
}


TransMembraneDomain::~TransMembraneDomain()
{
	
}

void TransMembraneDomain::SetVesicle(Vesicle* interior)
{
	mInterior = interior;
}


Vesicle* TransMembraneDomain::GetVesicle()
{
	return mInterior;
}


const Vesicle* TransMembraneDomain::GetVesicle() const
{
	return mInterior;
}
	
void TransMembraneDomain::IngressSolute(chemical::Solute* external)
{
	BIO_SANITIZE(mInterior,,return)
	BIO_SANITIZE(external,,return)
	mInterior->Influx(*external);
}

void TransMembraneDomain::IngressSolution(chemical::Solution* external)
{
	BIO_SANITIZE(mInterior,,return)
	BIO_SANITIZE(external,,return)
	for (
		SmartIterator slt = external->Begin();
		!slt.IsAfterEnd();
		++slt
	) {
		mInterior->Influx(external->Efflux(slt.As< chemical::Solute >().GetId()));
	}
}
	
chemical::Solute TransMembraneDomain::Egress(const Name& soluteName)
{
	BIO_SANITIZE(mInterior,,return NULL)
	return mInterior->Efflux(soluteName);
}

chemical::Solute TransMembraneDomain::Egress(const Id& soluteId)
{
	BIO_SANITIZE(mInterior,,return NULL)
	return mInterior->Efflux(soluteId);
}

chemical::Solute* TransMembraneDomain::Secrete(const Name& soluteName)
{
	return Secrete(IdPerspective::Instance().GetIdFromName(soluteName));
}

chemical::Solute* TransMembraneDomain::Secrete(const Id& soluteId)
{
	BIO_SANITIZE(mInterior,,return NULL)
	chemical::Solute toClone = mInterior->Efflux(soluteId);
	BIO_SANITIZE(toClone.GetConcentration() == 2,,return NULL) //very likely user error.
	toClone.SetEnvironment(NULL);
	chemical::Solute* toSecrete = ChemicalCast< chemical::Solute* >(toClone.Clone());
	mInterior->chemical::Solution::Erase(toClone.GetIndexInParentSolution());
	return toSecrete;
}

} //molecular namespace
} //bio namespace
