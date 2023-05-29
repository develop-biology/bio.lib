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

#include "bio/chemical/solution/Solution.h"
#include "bio/chemical/solution/Solute.h"

namespace bio {
namespace chemical {

Id Solution::Dissolve(
	Substance* toDissolve,
	const DiffusionTime& diffusionTime,
	const DiffusionEffort& diffusionEffort
)
{
	BIO_SANITIZE(toDissolve,,return IdPerspective::InvalidId())
	Index existingSolute = this->SeekToId(toDissolve->GetId());
	if (existingSolute)
	{
		mSolutes.LinearAccess(existingSolute)->AsAtom()->As< Solute* >()->MixWith(toDissolve);
		return toDissolve->GetId();
	}
	else
	{
		Solute* solute = new Solute(toDissolve->GetId());
		solute->mDissolvedSubstance = toDissolve;
		solute->SetDiffusionEffort(diffusionEffort);
		solute->SetDiffusionTime(diffusionTime);
		solute->SetEnvironment(this);
		this->physical::Line::Add(physical::Linear(solute, false)); //not shared.
		return solute->GetId();
	}
}

Substance* Solution::Separate(const Id& id)
{
	Index existingSolute = As< physical::Line* >()->SeekToId(id);
	BIO_SANITIZE(existingSolute,,return NULL)
	Solute* solute = mSolutes.LinearAccess(existingSolute)->AsAtom()->As< Solute* >();
	BIO_SANITIZE(solute && solute->GetConcentration() == 1,,return NULL) //likely user error.
	Substance* substance = solute->mDissolvedSubstance;
	solute->mDissolvedSubstance = NULL;
	solute->SetEnvironment(NULL);
	mSolutes.Erase(existingSolute);
	return substance;
}

Id Solution::Influx(const Solute& toInflux)
{
	Index existingSolute = this->SeekToId(toInflux.GetId());
	if (existingSolute)
	{
		Solute* solute = mSolutes.LinearAccess(existingSolute)->AsAtom()->As< Solute* >();
		solute->MixWith(toInflux);
		return solute->GetId();
	}
	Solute* toAdd = new Solute(toInflux);
	toAdd->SetEnvironment(this);
	mSolutes.Add(physical::Linear(toAdd, false)); //not shared.
	return toAdd->GetId();
}

Solute Solution::Efflux(const Id& soluteId)
{
	Index existingSolute = mSolutes.SeekToId(soluteId);
	BIO_SANITIZE(existingSolute,, return *new Solute())
	Solute* solute = mSolutes.LinearAccess(existingSolute)->AsAtom()->As< Solute* >();
	Solute ret = Solute(*solute);
	ret.SetEnvironment(NULL);
	return ret;
}

const Solute Solution::Efflux(const Id& soluteId) const
{
	Index existingSolute = mSolutes.SeekToId(soluteId);
	BIO_SANITIZE(existingSolute,, return *new Solute())
	const Solute* solute = mSolutes.LinearAccess(existingSolute)->AsAtom()->As< Solute* >();
	Solute ret = Solute(*solute);
	ret.mDissolvedSubstance = NULL; //read only; remove write access.
	ret.SetEnvironment(NULL);
	return ret;
}

Solute Solution::Efflux(const Name& substanceName)
{
	return Efflux(IdPerspective::Instance().GetIdFromName(substanceName));
}

const Solute Solution::Efflux(const Name& substanceName) const
{
	return Efflux(IdPerspective::Instance().GetIdFromName(substanceName));
}

Solute Solution::operator[](const Id& soluteId)
{
	return Efflux(soluteId);
}

const Solute Solution::operator[](const Id& soluteId) const
{
	return Efflux(soluteId);
}

Solute Solution::operator[](const Name& substanceName)
{
	return Efflux(substanceName);
}

const Solute Solution::operator[](const Name& substanceName) const
{
	return Efflux(substanceName);
}

} //chemical namespace
} //bio namespace
