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

#include "bio/chemical/solution/Solute.h"
#include "bio/chemical/solution/Solution.h"

namespace bio {
namespace chemical {

Solute::~Solute()
{

}

void Solute::CommonConstructor()
{
	mConcentration = 0;
	mParentSolution = NULL;
	mIndexInParentSolution = InvalidIndex();
}

Concentration Solute::GetConcentration() const
{
	return mConcentration;
}

void Solute::IncrementConcentration() const
{
	++mConcentration;
}

void Solute::DecrementConcentration() const //const required to decrement const Solutes.
{
	--mConcentration;
	if (!mConcentration)
	{
		const_cast< Solute* >(this)->Destructor();
	}
}

void Solute::SetConcentration(Concentration toSet) const
{
	mConcentration = toSet;
	if (!mConcentration)
	{
		const_cast< Solute* >(this)->Destructor();
	}
}

void Solute::Resolve() const
{
	if (mParentSolution && mIndexInParentSolution)
	{
		mParentSolution->Influx(*this);
	}
}

void Solute::SetEnvironment(Solution* environment)
{
	//Nothing need be done here.
	//In the past, we considered allowing each Solution to maintain its own Id <-> Name mapping (i.e. be a Perspective).
	//For that to be possible, we must set the Id of *this to match the name mapping of the environment.
	//However, this pattern was foregone in favor of Mix, Collapse, & Interference, which all allow Substances to be intelligently combined.
	//Now, if you would like to combine a Solute in one Solution with a Solute of a different Name in another Solution, you may engage the Mix machinery manually (i.e. just invoke Mix with both Solutes).
	this->EnvironmentDependent< Solution* >::SetEnvironment(environment);
}

Index Solute::GetIndexInParentSolution() const
{
	return mIndexInParentSolution;
}

void Solute::Destructor()
{
	if (mParentSolution && mIndexInParentSolution)
	{
		Resolve();
		mParentSolution->DecrementConcentration(mIndexInParentSolution);
	}
	Solution* env = GetEnvironment();
	if (env)
	{
		env->RemoveById< Solute* >(GetId());
	}
	delete this;
}

} //chemical namespace
} //bio namespace
