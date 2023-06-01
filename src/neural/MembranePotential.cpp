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

#include "bio/neural/MembranePotential.h"
#include "bio/neural/cell/StemCell.h"

namespace bio {
namespace neural {

MembranePotential::~MembranePotential()
{

}

unsigned int MembranePotential::GetPotential() const
{
	return mPotential;
}

void MembranePotential::SetPotential(unsigned int potential)
{
	mPotential = potential;
}

void MembranePotential::IncrementPotential()
{
	++mPotential;
}

void MembranePotential::DecrementPotential()
{
	--mPotential;
}

bool MembranePotential::ShouldReset() const
{
	return mShouldReset;
}

void MembranePotential::SetReset(bool should)
{
	mShouldReset = should;
}

molecular::Protein* MembranePotential::GetResetProtein()
{
	return mResetProtein;
}

void MembranePotential::SetResetProtein(molecular::Protein* protein)
{
	if (GetEnvironment())
	{
		mResetProtein->RecruitChaperones(GetEnvironment()->As< molecular::Vesicle* >());
		mResetProtein->Fold();
	}
	mResetProtein = protein;
}

void MembranePotential::SetEnvironment(bio::neural::StemCell* environment)
{
	if (mResetProtein)
	{
		mResetProtein->RecruitChaperones(environment->As< molecular::Vesicle* >());
		mResetProtein->Fold();
	}
	chemical::EnvironmentDependent< StemCell* >::SetEnvironment(environment);
}

Code MembranePotential::Reset()
{
	if (!ShouldReset() || mPotential != 0)
	{
		return code::NoErrorNoSuccess();
	}
	return (*mResetProtein)();
}

} //neural namespace
} //bio namespace
